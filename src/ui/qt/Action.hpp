#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <QAction>
#include <medusa/cell_action.hpp>
#include <medusa/disassembly_view.hpp> // TODO: include view.hpp instead when Selectionnable will be designed

class UiAction : public QAction
{
Q_OBJECT

public:
  UiAction(QObject* pParent, medusa::Action::SPtr spAction, QKeySequence const& rShortcut, medusa::FullDisassemblyView const* pView);

  bool IsCompatible(medusa::Action::RangeAddress const& rRangeAddress, medusa::u8 Index) const;
  void Do(medusa::Action::RangeAddress const& rRangeAddress, medusa::u8 Index);

private:
  medusa::Action::SPtr m_spAction;
  medusa::FullDisassemblyView const* m_pView;
};

#endif // !__ACTION_HPP__
