#include "ControlFlowGraphScene.hpp"

#include "BasicBlockItem.hpp"
#include "EdgeItem.hpp"

#include <list>

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

ControlFlowGraphScene::ControlFlowGraphScene(QObject * parent, medusa::Medusa& core, medusa::Address const& cfgAddr)
  : QGraphicsScene(parent)
  , _core(core)
{
  ogdf::Graph graph;
  ogdf::GraphAttributes graphAttr(graph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);


  medusa::ControlFlowGraph cfg;
  if (!core.BuildControlFlowGraph(cfgAddr, cfg))
  {
    medusa::Log::Write("ui_qt") << "failed to build CFG for: " << cfgAddr << medusa::LogEnd;
    return;
  }

  qreal maxBbWidth = 0.0, maxBbHeight = 0.0;

  QFontMetrics fm(font());
  std::map<ogdf::node,  BasicBlockItem*> nodes;
  std::map<ogdf::edge,  EdgeItem*      > edges;
  std::map<medusa::u64, ogdf::node     > usedBscBlock;
  auto addBscBlk = [&](medusa::u64 bbId) -> BasicBlockItem*
  {
    auto itBscBlk = usedBscBlock.find(bbId);
    if (itBscBlk != std::end(usedBscBlock))
      return nodes[ usedBscBlock[ bbId ] ];
      
    auto bbItem = new BasicBlockItem(this, _core, cfg.GetGraph()[bbId].GetAddresses());
    auto newNode = graph.newNode();
    auto rect = bbItem->boundingRect();
    graphAttr.width()[newNode]  = rect.width();
    graphAttr.height()[newNode] = rect.height();
    maxBbWidth  = std::max(maxBbWidth,  rect.width());
    maxBbHeight = std::max(maxBbHeight, rect.height());
    nodes[newNode]     = bbItem;
    usedBscBlock[bbId] = newNode;
    return bbItem;
  };

  auto const& g = cfg.GetGraph();

  auto vertexRange = boost::vertices(g);
  for (auto vertexIter = vertexRange.first; vertexIter != vertexRange.second; ++vertexIter)
    addBscBlk(*vertexIter);

  auto edgeRange = boost::edges(g);
  for (auto edgeIter = edgeRange.first; edgeIter != edgeRange.second; ++edgeIter)
  {
    auto srcBb     = addBscBlk(edgeIter->m_source);
    auto tgtBb     = addBscBlk(edgeIter->m_target);
    auto newEdge   = graph.newEdge(usedBscBlock[edgeIter->m_source], usedBscBlock[edgeIter->m_target]);
    auto edgeItem  = new EdgeItem(srcBb, tgtBb, g[*edgeIter].GetType());
    edges[newEdge] = edgeItem;
  }

  auto OHL = new ogdf::OptimalHierarchyLayout;
  OHL->nodeDistance(25.0);
  OHL->layerDistance(50.0);
  OHL->weightBalancing(0.0);
  OHL->weightSegments(0.0);

  ogdf::SugiyamaLayout SL;
  SL.setRanking(new ogdf::OptimalRanking);
  SL.setCrossMin(new ogdf::MedianHeuristic);
  SL.alignSiblings(false);
  SL.setLayout(OHL);
  SL.call(graphAttr);

  for (auto nodeIter = std::begin(nodes); nodeIter != std::end(nodes); ++nodeIter)
  {
    addItem(nodeIter->second);
    QRectF bbRect = nodeIter->second->boundingRect();
    qreal x = graphAttr.x(nodeIter->first) - (bbRect.width()  / 2);
    qreal y = graphAttr.y(nodeIter->first) - (bbRect.height() / 2);
    nodeIter->second->setPos(x, y);
  }

  for (auto edgeIter = std::begin(edges); edgeIter != std::end(edges); ++edgeIter)
  {
    auto const& bends = graphAttr.bends(edgeIter->first);
    edgeIter->second->setBends(bends);
    addItem(edgeIter->second);
  }

  setSceneRect(0, 0, graphAttr.boundingBox().width() + maxBbWidth, graphAttr.boundingBox().height() + maxBbHeight);
}