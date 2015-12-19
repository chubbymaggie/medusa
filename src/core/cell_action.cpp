#include "medusa/cell_action.hpp"
#include "medusa/module.hpp"
#include "medusa/log.hpp"
#include <boost/bind.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

MEDUSA_NAMESPACE_USE

namespace
{

class CellAction_Undefine : public Action
{
public:
  CellAction_Undefine(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_Undefine>(rCore); }

  virtual std::string GetName(void) const
  { return "Undefine"; }

  static char const* GetBindingName(void)
  { return "action.undefine"; }

  virtual std::string GetDescription(void) const
  { return "This option converts the selected item to byte"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    m_rCore.GetDocument().DeleteCell(rRangeAddress.second);
  }
};

class CellAction_ToWord : public Action
{
public:
  CellAction_ToWord(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ToWord>(rCore); }

  static char const* GetBindingName(void)
  { return "action.to_word"; }

  virtual std::string GetName(void) const
  { return "Set to word"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to word type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    m_rCore.GetDocument().ChangeValueSize(rRangeAddress.second, 16, true);
  }
};

class CellAction_ToDword : public Action
{
public:
  CellAction_ToDword(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ToDword>(rCore); }

  static char const* GetBindingName(void)
  { return "action.to_dword"; }

  virtual std::string GetName(void) const
  { return "Set to dword"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to dword type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    m_rCore.GetDocument().ChangeValueSize(rRangeAddress.second, 32, true);
  }
};

class CellAction_ToQword : public Action
{
public:
  CellAction_ToQword(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ToQword>(rCore); }

  static char const* GetBindingName(void)
  { return "action.to_qword"; }

  virtual std::string GetName(void) const
  { return "Set to qword"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to qword type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    m_rCore.GetDocument().ChangeValueSize(rRangeAddress.second, 64, true);
  }
};

class CellAction_ChangeValueSize : public Action
{
public:
  CellAction_ChangeValueSize(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ChangeValueSize>(rCore); }

  static char const* GetBindingName(void)
  { return "action.change_value_size"; }

  virtual std::string GetName(void) const
  { return "Change value size"; }

  virtual std::string GetDescription(void) const
  { return "This option allows to change the size of a value (byte, word, dword, qword)"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    auto rAddr = rRangeAddress.second;
    {
      auto pCell = m_rCore.GetCell(rAddr);
      if (pCell == nullptr) return;

      u8 NewSize = 0;
      switch (pCell->GetLength())
      {
      case 1: NewSize = 2; break;
      case 2: NewSize = 4; break;
      case 4: NewSize = 8; break;
      case 8: NewSize = 1; break;
      default: return;
      }

      m_rCore.GetDocument().ChangeValueSize(rAddr, NewSize * 8, true);
    }
  }
};

class CellAction_ToCharacter : public Action
{
public:
  CellAction_ToCharacter(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ToCharacter>(rCore); }

  static char const* GetBindingName(void)
  { return "action.to_character"; }

  virtual std::string GetName(void) const
  { return "To character"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to character type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    auto rAddr = rRangeAddress.second;
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      spCell->SubType() &= ValueDetail::ModifierMask;
      spCell->SubType() |= ValueDetail::CharacterType;
      m_rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_ToReference : public Action
{
public:
  CellAction_ToReference(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ToReference>(rCore); }

  static char const* GetBindingName(void)
  { return "action.to_reference"; }

  virtual std::string GetName(void) const
  { return "To reference"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to reference type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    auto rAddr = rRangeAddress.second;
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      spCell->SubType() &= ValueDetail::ModifierMask;
      spCell->SubType() |= ValueDetail::ReferenceType;
      m_rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_Not: public Action
{
public:
  CellAction_Not(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_Not>(rCore); }

  static char const* GetBindingName(void)
  { return "action.not_value"; }

  virtual std::string GetName(void) const
  { return "Not"; }

  virtual std::string GetDescription(void) const
  { return "Not the value"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    auto rAddr = rRangeAddress.second;
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      spCell->SubType() &= ~ValueDetail::ModifierMask;
      spCell->SubType() ^= ValueDetail::NotType;
      m_rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_Negate : public Action
{
public:
  CellAction_Negate(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_Negate>(rCore); }

  static char const* GetBindingName(void)
  { return "action.negate_value"; }

  virtual std::string GetName(void) const
  { return "Negate"; }

  virtual std::string GetDescription(void) const
  { return "Negate the value"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    auto rAddr = rRangeAddress.second;
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      u8 SubType = spCell->GetSubType();
      spCell->SubType() &= ~ValueDetail::ModifierMask;
      spCell->SubType() ^= ValueDetail::NegateType;
      m_rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_Normal : public Action
{
public:
  CellAction_Normal(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_Normal>(rCore); }

  static char const* GetBindingName(void)
  { return "action.reset_value"; }

  virtual std::string GetName(void) const
  { return "Normal"; }

  virtual std::string GetDescription(void) const
  { return "Reset value modifier"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    auto rAddr = rRangeAddress.second;
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      u8 SubType = spCell->GetSubType();
      spCell->SubType() &= ~ValueDetail::ModifierMask;
      m_rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_Analyze : public Action
{
public:
  CellAction_Analyze(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_Analyze>(rCore); }

  static char const* GetBindingName(void)
  { return "action.analyze"; }

  virtual std::string GetName(void) const
  { return "Analyze"; }

  virtual std::string GetDescription(void) const
  { return "Analyze using the most appropriate architecture"; }

  virtual std::string GetIconName(void) const
  { return "analyze.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    // TODO: iterate
    auto rAddr = rRangeAddress.second;
    {
      auto spArch = ModuleManager::Instance().GetArchitecture(m_rCore.GetDocument().GetArchitectureTag(rAddr));
      u8 Mode = m_rCore.GetDocument().GetMode(rAddr);
      if (spArch == nullptr)
      {
        Log::Write("core") << "unable to get architecture for " << rAddr << LogEnd;
        return;
      }
      m_rCore.Analyze(rAddr, spArch, Mode);
    }
  }
};

class CellAction_CreateFunction : public Action
{
public:
  CellAction_CreateFunction(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_CreateFunction>(rCore); }

  static char const* GetBindingName(void)
  { return "action.create_function"; }

  virtual std::string GetName(void) const
  { return "Create function"; }

  virtual std::string GetDescription(void) const
  { return "Create a new function from the current address"; }

  virtual std::string GetIconName(void) const
  { return "function.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    m_rCore.CreateFunction(rRangeAddress.second);
  }
};

class CellAction_ToUtf8String : public Action
{
public:
  CellAction_ToUtf8String(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ToUtf8String>(rCore); }

  static char const* GetBindingName(void)
  { return "action.to_utf8_string"; }

  virtual std::string GetName(void) const
  { return "To UTF-8 string"; }

  virtual std::string GetDescription(void) const
  { return "Make an UTF-8 string"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    //Address OldAddr;
    //u64 StrLen = 0;
    //for (auto const& rAddr : rAddrList)
    //{
    //  if (OldAddr + StrLen <= rAddr)
    //  {
    //    m_rCore.MakeAsciiString(rAddr);
    //    auto pStr = m_rCore.GetCell(rAddr);
    //    if (pStr == nullptr) return;
    //    OldAddr = rAddr;
    //    StrLen = pStr->GetLength();
    //  }
    //}
    // TODO: iterate
    m_rCore.MakeAsciiString(rRangeAddress.second);
  }
};

class CellAction_ToUtf16String : public Action
{
public:
  CellAction_ToUtf16String(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_ToUtf16String>(rCore); }

  static char const* GetBindingName(void)
  { return "action.to_utf16_name"; }

  virtual std::string GetName(void) const
  { return "To UTF-16 string"; }

  virtual std::string GetDescription(void) const
  { return "Make an UTF-16 string"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    //Address OldAddr;
    //u64 StrLen = 0;
    //for (auto const& rAddr : rAddrList)
    //{
    //  if (OldAddr + StrLen <= rAddr)
    //  {
    //    m_rCore.MakeWindowsString(rAddr);
    //    auto pStr = m_rCore.GetCell(rAddr);
    //    if (pStr == nullptr) return;
    //    OldAddr = rAddr;
    //    StrLen = pStr->GetLength();
    //  }
    //}
    // TODO: iterate
    m_rCore.MakeWindowsString(rRangeAddress.second);
  }
};


class CellAction_GoToPreviousAddress : public Action
{
public:
  CellAction_GoToPreviousAddress(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_GoToPreviousAddress>(rCore); }

  static char const* GetBindingName(void)
  { return "action.go_to_previous_address"; }

  virtual std::string GetName(void) const
  { return "Go to previous address"; }

  virtual std::string GetDescription(void) const
  { return "Get back to the previous address"; }

  virtual std::string GetIconName(void) const
  { return "previous_address.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    auto& rDoc = m_rCore.GetDocument();
    Address PrevAddr;
    if (!rDoc.GetPreviousAddressInHistory(PrevAddr))
    {
      Log::Write("core") << "unable to get previous address" << LogEnd;
      return;
    }

    Log::Write("core") << "previous address: " << PrevAddr << LogEnd;
  }
};

class CellAction_GoToNextAddress : public Action
{
public:
  CellAction_GoToNextAddress(Medusa& rCore) : Action(rCore) {}

  static SPtr Create(Medusa& rCore)
  { return std::make_shared<CellAction_GoToNextAddress>(rCore); }

  static char const* GetBindingName(void)
  { return "action.go_to_next_address"; }

  virtual std::string GetName(void) const
  { return "Go to next address"; }

  virtual std::string GetDescription(void) const
  { return "Go to the next address"; }

  virtual std::string GetIconName(void) const
  { return "next_address.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    auto& rDoc = m_rCore.GetDocument();
    Address NextAddr;
    if (!rDoc.GetNextAddressInHistory(NextAddr))
    {
      Log::Write("core") << "unable to get next address" << LogEnd;
      return;
    }

    Log::Write("core") << "next address: " << NextAddr << LogEnd;
  }
};

}

Action::MapType Action::GetMap(void)
{
  static MapType s_Actions;
  if (s_Actions.empty())
  {
    s_Actions[CellAction_Undefine::GetBindingName()]            = &CellAction_Undefine::Create;
    s_Actions[CellAction_ChangeValueSize::GetBindingName()]     = &CellAction_ChangeValueSize::Create;
    s_Actions[CellAction_ToWord::GetBindingName()]              = &CellAction_ToWord::Create;
    s_Actions[CellAction_ToDword::GetBindingName()]             = &CellAction_ToDword::Create;
    s_Actions[CellAction_ToQword::GetBindingName()]             = &CellAction_ToQword::Create;
    s_Actions[CellAction_ToReference::GetBindingName()]         = &CellAction_ToReference::Create;
    s_Actions[CellAction_ToCharacter::GetBindingName()]         = &CellAction_ToCharacter::Create;
    s_Actions[CellAction_Normal::GetBindingName()]              = &CellAction_Normal::Create;
    s_Actions[CellAction_Not::GetBindingName()]                 = &CellAction_Not::Create;
    s_Actions[CellAction_Negate::GetBindingName()]              = &CellAction_Negate::Create;
    s_Actions[CellAction_Analyze::GetBindingName()]             = &CellAction_Analyze::Create;
    s_Actions[CellAction_CreateFunction::GetBindingName()]      = &CellAction_CreateFunction::Create;
    s_Actions[CellAction_ToUtf8String::GetBindingName()]        = &CellAction_ToUtf8String::Create;
    s_Actions[CellAction_ToUtf16String::GetBindingName()]       = &CellAction_ToUtf16String::Create;
    s_Actions[CellAction_GoToPreviousAddress::GetBindingName()] = &CellAction_GoToPreviousAddress::Create;
    s_Actions[CellAction_GoToNextAddress::GetBindingName()]     = &CellAction_GoToNextAddress::Create;
  }
  return s_Actions;
}

class CellAction_AnalyzeWith : public Action
{
public:
  CellAction_AnalyzeWith(Medusa& rCore, Tag ArchitectureTag, Architecture::NamedMode& rNamedMode)
    : Action(rCore), m_ArchTag(ArchitectureTag), m_NamedMode(rNamedMode) {}

  static SPtr Create(Medusa& rCore)
  { return nullptr; }

  static char const* GetBindingName(void)
  { return nullptr; }

  virtual std::string GetName(void) const
  { return std::string("Analyze with ") + std::get<0>(m_NamedMode); }

  virtual std::string GetDescription(void) const
  { return std::string("Analyze using the mode ") + std::get<0>(m_NamedMode); }

  virtual std::string GetIconName(void) const
  { return "analyze.png"; }

  virtual bool IsCompatible(RangeAddress const& rRangeAddress, u8 Index) const
  { return true; }

  virtual void Do(RangeAddress const& rRangeAddress, u8 Index)
  {
    auto spArch = ModuleManager::Instance().GetArchitecture(m_ArchTag);
    if (spArch == nullptr)
    {
      Log::Write("core") << "unable to retrieve arch using tag: " << m_ArchTag << LogEnd;
      return;
    }

    // TODO: iterate...
    u8 Mode = std::get<1>(m_NamedMode);
    m_rCore.Analyze(rRangeAddress.second, spArch, Mode);
  }

protected:
  Tag m_ArchTag;
  Architecture::NamedMode m_NamedMode;
};

Action::SPtrList Action::GetSpecificActions(Medusa& rCore, Address const& rAddress)
{
  SPtrList Actions;

  auto& rModMgr = ModuleManager::Instance();

  auto const ArchTags = rCore.GetDocument().GetArchitectureTags();
  for (auto ArchTag : ArchTags)
  {
    auto spArch = rModMgr.GetArchitecture(ArchTag);
    if (spArch == nullptr)
      continue;

    auto ArchModes = spArch->GetModes();

    for (auto& rArchMode : ArchModes)
    {
      Actions.push_back(std::make_shared<CellAction_AnalyzeWith>(rCore, ArchTag, rArchMode));
    }
  }

  return Actions;
}