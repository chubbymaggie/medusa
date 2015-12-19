#include <ios>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>
#include <boost/foreach.hpp>

#include "boost/graph/graphviz.hpp"

#include <medusa/configuration.hpp>
#include <medusa/address.hpp>
#include <medusa/medusa.hpp>
#include <medusa/document.hpp>
#include <medusa/memory_area.hpp>
#include <medusa/log.hpp>
#include <medusa/event_handler.hpp>
#include <medusa/disassembly_view.hpp>
#include <medusa/execution.hpp>
#include <medusa/module.hpp>

MEDUSA_NAMESPACE_USE

  std::ostream& operator<<(std::ostream& out, std::pair<u32, std::string> const& p)
{
  out << p.second;
  return out;
}

class DummyEventHandler : public EventHandler
{
public:
  virtual bool OnDocumentUpdated(void)
  {
    return true;
  }
};

template<typename Type, typename Container>
class AskFor
{
public:
  Type operator()(Container const& c)
  {
    if (c.empty())
      throw std::runtime_error("Nothing to ask!");

    while (true)
    {
      size_t Count = 0;
      for (typename Container::const_iterator i = c.begin(); i != c.end(); ++i)
      {
        std::cout << Count << " " << (*i)->GetName() << std::endl;
        Count++;
      }
      size_t Input;
      std::cin >> Input;

      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      if (Input < c.size())
        return c[Input];
    }
  }
};

struct AskForConfiguration : public boost::static_visitor<>
{
  AskForConfiguration(ConfigurationModel& rCfgMdl) : m_rCfgMdl(rCfgMdl) {}

  ConfigurationModel& m_rCfgMdl;

  void operator()(ConfigurationModel::NamedBool const& rBool) const
  {
    std::cout << rBool.GetName() << " " << rBool.GetValue() << std::endl;
    std::cout << "true/false" << std::endl;

    while (true)
    {
      u32 Choose;
      std::string Result;

      std::getline(std::cin, Result, '\n');

      if (Result.empty()) return;

      if (Result == "false" || Result == "true")
      {
        m_rCfgMdl.SetEnum(rBool.GetName(), !!(Result == "true"));
        return;
      }

      std::istringstream iss(Result);
      if (!(iss >> Choose)) continue;

      if (Choose == 0 || Choose == 1)
      {
        m_rCfgMdl.SetEnum(rBool.GetName(), Choose);
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedEnum const& rEnum) const
  {
    std::cout << std::dec;
    std::cout << "ENUM TYPE: " << rEnum.GetName() << std::endl;
    for (Configuration::Enum::const_iterator It = rEnum.GetConfigurationValue().begin();
      It != rEnum.GetConfigurationValue().end(); ++It)
    {
      if (It->second == m_rCfgMdl.GetEnum(rEnum.GetName()))
        std::cout << "* ";
      else
        std::cout << "  ";
      std::cout << It->first << ": " << It->second << std::endl;
    }

    while (true)
    {
      u32 Choose;
      std::string Result;

      std::getline(std::cin, Result, '\n');

      if (Result.empty()) return;

      std::istringstream iss(Result);
      if (!(iss >> Choose)) continue;

      for (Configuration::Enum::const_iterator It = rEnum.GetConfigurationValue().begin();
        It != rEnum.GetConfigurationValue().end(); ++It)
        if (It->second == Choose)
        {
          m_rCfgMdl.SetEnum(rEnum.GetName(), Choose);
          return;
        }
    }
  }
};

void DummyLog(std::string const & rMsg)
{
  std::wcout << rMsg << std::flush;
}

int main(int argc, char **argv)
{
  std::cout.sync_with_stdio(false);
  std::wcout.sync_with_stdio(false);
  boost::filesystem::path file_path;
  boost::filesystem::path mod_path(".");
  Log::SetLog(DummyLog);

  try
  {
    if (argc != 2)
      return 0;
    file_path = argv[1];

    std::wcout << L"Analyzing the following file: \""         << file_path << "\"" << std::endl;
    std::wcout << L"Using the following path for modules: \"" << mod_path  << "\"" << std::endl;

    BinaryStream::SharedPtr bin_strm = std::make_shared<FileBinaryStream>(file_path);
    Medusa m;

    auto& mod_mgr = ModuleManager::Instance();

    mod_mgr.LoadModules(L".", *bin_strm);

    if (mod_mgr.GetLoaders().empty())
    {
      std::cerr << "Not loader available" << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Choose a executable format:" << std::endl;
    AskFor<Loader::VectorSharedPtr::value_type, Loader::VectorSharedPtr> AskForLoader;
    Loader::VectorSharedPtr::value_type ldr = AskForLoader(mod_mgr.GetLoaders());
    std::cout << "Interpreting executable format using \"" << ldr->GetName() << "\"..." << std::endl;
    std::cout << std::endl;

    auto archs = mod_mgr.GetArchitectures();
    ldr->FilterAndConfigureArchitectures(archs);
    auto os = mod_mgr.GetOperatingSystem(ldr, archs.front());

    std::cout << std::endl;

    ConfigurationModel CfgMdl;

    std::cout << "Configuration:" << std::endl;
    for (auto itCfg = CfgMdl.Begin(), itEnd = CfgMdl.End(); itCfg != itEnd; ++itCfg)
      boost::apply_visitor(AskForConfiguration(CfgMdl), itCfg->second);

    AskFor<Database::VectorSharedPtr::value_type, Database::VectorSharedPtr> AskForDb;
    auto db = AskForDb(mod_mgr.GetDatabases());

    boost::filesystem::path db_path = file_path;
    db_path.replace_extension(db->GetExtension());
    if (db->Create(db_path, false) == false)
      db->Open(db_path);

    if (ldr->GetName() == "Raw file")
      db->AddLabel(0x0, Label("start", Label::Code | Label::Exported));

    m.Start(bin_strm, db, ldr, archs, os);
    std::cout << "Disassembling..." << std::endl;
    m.WaitForTasks();

    Execution exec(m.GetDocument(), archs.front(), os);
    if (!exec.Initialize(m.GetDocument().GetMode(m.GetDocument().GetStartAddress()), 0x2000000, 0x40000))
    {
      std::cerr << "Unable to initialize emulator" << std::endl;
      return 0;
    }
    if (!exec.SetEmulator("interpreter"))
    {
      std::cerr << "Unable to set the emulator" << std::endl;
      return 0;
    }

    exec.Execute(m.GetDocument().GetAddressFromLabelName("start"));

    //auto fnApiStub = [](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
    //{
    //  std::cout << "API called! Let's gtfo..." << std::endl;
    //  auto RegPc = pCpuCtxt->GetCpuInformation().GetRegisterByType(CpuInformation::ProgramPointerRegister);
    //  auto RegSp = pCpuCtxt->GetCpuInformation().GetRegisterByType(CpuInformation::StackPointerRegister);
    //  auto RegSz = pCpuCtxt->GetCpuInformation().GetSizeOfRegisterInBit(RegPc) / 8;
    //  u64 RegSpValue = 0x0, RetAddr = 0x0;
    //  pCpuCtxt->ReadRegister(RegSp, &RegSpValue, RegSz);
    //  pMemCtxt->ReadMemory(RegSpValue, &RetAddr, RegSz);
    //  RegSpValue += RegSz;
    //  pCpuCtxt->WriteRegister(RegSp, &RegSpValue, RegSz);
    //  pCpuCtxt->WriteRegister(RegPc, &RetAddr, RegSz);
    //};

    //static char const* apis_name[] =
    //{
    //  "kernel32.dll!GetSystemTimeAsFileTime",
    //  "kernel32.dll!GetCurrentThreadId",
    //  "kernel32.dll!GetTickCount64",
    //  "kernel32.dll!QueryPerformanceCounter",
    //  "msvcr110.dll!__crtGetShowWindowMode",
    //  "kernel32.dll!GetCurrentProcessId",
    //  "kernel32.dll!GetTickCount",
    //  "kernel32.dll!Sleep",
    //  "msvcrt.dll!_initterm",
    //  "msvcr110.dll!_initterm_e",
    //  "msvcr110.dll!_initterm",
    //  "msvcr110d.dll!_initterm_e",
    //  "kernel32.dll!GetProcessHeap",
    //  "kernel32.dll!EncodePointer",
    //  "kernel32.dll!InitializeCriticalSectionAndSpinCount",
    //  "kernel32.dll!FlsAlloc",
    //  "kernel32.dll!SetUnhandledExceptionFilter",
    //  "kernel32.dll!LoadLibraryW",
    //  "kernel32.dll!GetProcAddress",
    //  "msvcrt.dll!malloc",
    //  "msvcrt.dll!_lock",
    //  "msvcrt.dll!__dllonexit",
    //  "msvcrt.dll!_unlock",
    //  "msvcrt.dll!rand",
    //  "msvcrt.dll!puts",
    //  "msvcrt.dll!exit",
    //  "kernel32.dll!GetStartupInfoA",
    //  "kernel32.dll!InterlockedCompareExchange",
    //  "kernel32.dll!HeapSetInformation",
    //  "kernel32.dll!FindFirstFileA",
    //  "kernel32.dll!FindNextFileA",
    //  "kernel32.dll!GetStdHandle",
    //  "kernel32.dll!WriteFile",
    //  "kernel32.dll!CreateFileA",
    //  "kernel32.dll!CreateFileMappingA",
    //  "kernel32.dll!MapViewOfFile",
    //  "kernel32.dll!UnmapViewOfFile",
    //  "kernel32.dll!CloseHandle",
    //  "kernel32.dll!GetFileSize",
    //  "kernel32.dll!SetFilePointer",
    //  "kernel32.dll!SetEndOfFile",
    //  nullptr
    //};

    //u64 FakeValue = 0x9999999999999999ULL;
    //for (auto api_name = apis_name; *api_name; ++api_name)
    //{
    //  Address ApiAddr = m.GetDocument().GetAddressFromLabelName(*api_name);
    //  interp->WriteMemory(ApiAddr, &FakeValue, reg_sz);
    //}
    //interp->AddHook(Address(FakeValue), Emulator::HookOnExecute, fnApiStub);
    //interp->AddHook(Address(FakeValue & 0xffffffff), Emulator::HookOnExecute, fnApiStub);

    return 0;

    //ControlFlowGraph cfg;
    //m.BuildControlFlowGraph(faddr, cfg);
    //cfg.Dump("test.gv", m.GetDocument());
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (Exception& e)
  {
    std::wcerr << e.What() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
