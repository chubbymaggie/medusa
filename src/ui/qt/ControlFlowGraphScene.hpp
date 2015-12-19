#ifndef __CFG_SCENE_HPP__
#define __CFG_SCENE_HPP__

#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

#include <medusa/medusa.hpp>
#include <medusa/function.hpp>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

class ControlFlowGraphScene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit ControlFlowGraphScene(QObject * parent, medusa::Medusa& core, medusa::Address const& cfgAddr);

private:
  medusa::Medusa&         _core;
};

#endif // __CFG_SCENE_HPP__