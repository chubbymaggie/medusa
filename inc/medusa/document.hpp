#ifndef _MEDUSA_DOCUMENT_
#define _MEDUSA_DOCUMENT_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/cell.hpp"
#include "medusa/multicell.hpp"
#include "medusa/memory_area.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/xref.hpp"
#include "medusa/label.hpp"
#include "medusa/event_queue.hpp"

#include <set>
#include <boost/bimap.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

MEDUSA_NAMESPACE_BEGIN

//! Document handles cell, multicell, xref, label and memory area.
class Medusa_EXPORT Document
{
public:
  typedef std::set<MemoryArea*, MemoryArea::Compare>                     MemoryAreaSetType;
  typedef MemoryAreaSetType::iterator                                    TIterator;
  typedef MemoryAreaSetType::const_iterator                              TConstIterator;
  typedef boost::bimap<Address, Label>                                   LabelBimapType;
  typedef boost::signals2::connection                                    ConnectionType;


  class Medusa_EXPORT Subscriber
  {
    friend class Document;

  public:
    enum Type
    {
      Quit            = 1 << 0,
      DocumentUpdated = 1 << 1,
      LabelUpdated    = 1 << 2
    };

    virtual ~Subscriber(void)
    {
      m_QuitConnection.disconnect();
      m_DocumentUpdatedConnection.disconnect();
      m_LabelUpdatedConnection.disconnect();
    }

  private:
    typedef boost::signals2::signal<void (void)>                              QuitSignalType;
    typedef boost::signals2::signal<void (void)>                              DocumentUpdatedSignalType;
    typedef boost::signals2::signal<void (Label const& rLabel, bool Removed)> LabelUpdatedSignalType;

    typedef QuitSignalType::slot_type                                         QuitSlotType;
    typedef DocumentUpdatedSignalType::slot_type                              DocumentUpdatedSlotType;
    typedef LabelUpdatedSignalType::slot_type                                 LabelUpdatedSlotType;

    Document::ConnectionType m_QuitConnection;
    Document::ConnectionType m_DocumentUpdatedConnection;
    Document::ConnectionType m_LabelUpdatedConnection;

  public:
    virtual void OnQuit(void) {}
    virtual void OnDocumentUpdated(void) {}
    virtual void OnLabelUpdated(Label const& rLabel, bool Removed) {}
  };

                                /*!
                                 * The constructor needs a FileBinaryStream
                                 * \param rBinaryStream must contains the disassembled file.
                                 */
                                Document(FileBinaryStream const& rBinaryStream);
                                ~Document(void);

                                //! This method remove all memory areas.
  void                          RemoveAll(void);

  // Subscriber

  void                          Connect(u32 Type, Subscriber* pSubscriber);

  // Memory Area

                                /*! This method adds a new memory area.
                                 * \param pMemoryArea is the added memory area.
                                 */
  void                          AddMemoryArea(MemoryArea* pMemoryArea);

                                /*! This method return a specific memory area.
                                 * \param Addr is a address contained in the returned memory area.
                                 */
  MemoryArea*                   GetMemoryArea(Address const& Addr);
  MemoryArea const*             GetMemoryArea(Address const& Addr) const;

  // Binary Stream
  FileBinaryStream const&       GetFileBinaryStream(void) const { return m_rBinaryStream; }

  // Label
                                //! This method returns a label by its address.
  Label                         GetLabelFromAddress(Address const& rAddr) const;

                                //! This method update a label by its address.
  void                          SetLabelToAddress(Address const& rAddr, Label const& rLabel);

                                //! This method returns the address of rLabel.
  Address                       GetAddressFromLabelName(std::string const& rLabel) const;

                                //! This method add a new label.
  void                          AddLabel(Address const& rAddr, Label const& rLabel, bool Force = true);
  void                          RemoveLabel(Address const& rAddr);

                                //! This method returns all labels.
  LabelBimapType const&         GetLabels(void) const { return m_LabelMap; }

  // Xref

                                //! This method returns all cross-references.
  XRefs&                        GetXRefs(void) { return m_XRefs; }
  XRefs const&                  GetXRefs(void) const { return m_XRefs; }

  // Cell

                                /*! This method returns a cell by its address.
                                 * \return A pointer to a cell if the rAddr is valid, nullptr otherwise.
                                 */
  Cell*                         RetrieveCell(Address const& rAddr);
  Cell const*                   RetrieveCell(Address const& rAddr) const;

                                /*! This method adds a new cell.
                                 * \param rAddr is the address of the new cell.
                                 * \param pCell is the new cell.
                                 * \param Force makes the old cell to be deleted.
                                 * \param Safe makes this method to avoid "cell overlay" but it's slower.
                                 * \return Returns true if the new cell is added, otherwise it returns false.
                                 */
  bool                          InsertCell(Address const& rAddr, Cell* pCell, bool Force = false, bool Safe = true);

  void                          UpdateCell(Address const& rAddr, Cell* pCell);

                                //! Returns true if rAddr is contained in the Document.
  bool                          IsPresent(Address const& rAddr) const;
  bool                          IsPresent(Address::SharedPtr spAddr) const { return IsPresent(*spAddr.get()); }

                                //! Returns true if rAddr contains code.
  bool                          ContainsCode(Address const& rAddr) const
  {
    Cell const* pCell = RetrieveCell(rAddr);
    if (pCell == nullptr) return false;
    return pCell->GetType() == CellData::InstructionType;
  }

                                //! Returns true if rAddr contains data.
  bool                          ContainsData(Address const& rAddr) const
  {
    Cell const* pCell = RetrieveCell(rAddr);
    if (pCell == nullptr) return false;
    return pCell->GetType() == CellData::ValueType;
  }

  // Value

                                /*! Change size of object Value
                                 *  \param rValueAddr is the address of value
                                 *  \param NewValueSize must be 8 or 16 or 32 or 64
                                 *  \param Force makes this method to erase others cells if needed
                                 */
  bool                          ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force = false);

  // MultiCell

                                //! \return Returns a pointer to a multicell if rAddr is valid, otherwise nullptr.
  MultiCell*                    RetrieveMultiCell(Address const& rAddr);
  MultiCell const*              RetrieveMultiCell(Address const& rAddr) const;

                                /*! This method adds a new MultiCell.
                                 *  \param rAddr is the address of the MultiCell.
                                 *  \param pMultiCell is a the new MultiCell.
                                 *  \param Force removes the old MultiCell if set.
                                 *  \return Returns true if the new multicell is added, otherwise it returns false.
                                 */
  bool                          InsertMultiCell(Address const& rAddr, MultiCell* pMultiCell, bool Force = true);

                                /*! This method returns all couple Address and MultiCell
                                */
  MultiCell::Map const&         GetMultiCells(void) const { return m_MultiCells; }

  // Address

                                /*! This method makes an Address.
                                 *  \param Base is the base address.
                                 *  \param Offset is the offset address.
                                 *  \return Returns a shared pointer to a new Address with correct information if base and offset are associated to a memory area, otherwise it returns an empty shared pointer Address.
                                 */
  Address                       MakeAddress(TBase Base, TOffset Offset) const
  {
    MemoryArea const* ma = GetMemoryArea(Address(Base, Offset));
    if (ma == nullptr)
      return Address();
    return ma->MakeAddress(Offset);
  }

                                /*! This method translates an Address to a offset (file relative).
                                 * \return Returns true if the translation is possible, otherwise it returns false.
                                 */
  bool                          Translate(Address const& rAddr, TOffset& rRawOffset) const;

                                /*! This method converts an Address to a offset (memory area relative).
                                 * \return Returns true if the conversion is possible, otherwise it returns false.
                                 */
  bool                          Convert(Address const& rAddr, TOffset& rMemAreaOffset) const;

  // Data

  // Iterator
  TIterator                     Begin(void)       { return m_MemoryAreas.begin(); }
  TIterator                     End(void)         { return m_MemoryAreas.end();   }

  TConstIterator                Begin(void) const { return m_MemoryAreas.begin(); }
  TConstIterator                End(void)   const { return m_MemoryAreas.end();   }

  bool                          Read(Address const& rAddress, void* pBuffer, u32 Size) const;
  bool                          Write(Address const& rAddress, void const* pBuffer, u32 Size);

  u32                           GetNumberOfAddress(void) const;
  bool                          MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  bool                          MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  bool                          MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  bool                          GetNextAddress(Address const& rAddress, Address& rNextAddress) const;
  bool                          GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const;
  bool                          ConvertPositionToAddress(u64 Position, Address& rAddress) const;
  bool                          ConvertAddressToPosition(Address const& rAddress, u64& rPosition) const;

  void                          FindFunctionAddressFromAddress(Address::List& rFunctionAddress, Address const& rAddress) const;

  Address                       GetTheLastAddressAccessed(void) const;
  void                          SetTheLastAddressAccessed(Address const& rAddr);

private:
  void RemoveLabelIfNeeded(Address const& rAddr);

  typedef boost::mutex                  MutexType;

  FileBinaryStream const&               m_rBinaryStream;
  MemoryAreaSetType                     m_MemoryAreas;
  MultiCell::Map                        m_MultiCells;
  LabelBimapType                        m_LabelMap;
  XRefs                                 m_XRefs;
  mutable MutexType                     m_MemoryAreaMutex;
  mutable MutexType                     m_CellMutex;
  boost::thread                         m_Thread;
  Address                               m_LastAddressAccessed;
  Subscriber::QuitSignalType            m_QuitSignal;
  Subscriber::DocumentUpdatedSignalType m_DocumentUpdatedSignal;
  Subscriber::LabelUpdatedSignalType    m_LabelUpdatedSignal;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_DOCUMENT_
