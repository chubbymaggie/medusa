#include "medusa/bits.hpp"
#include "medusa/expression_visitor.hpp"
#include "medusa/expression_filter.hpp"

MEDUSA_NAMESPACE_BEGIN

Expression::SPType ExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  auto& rExprs = spBindExpr->GetBoundExpressions();
  for (auto pExpr : rExprs)
    pExpr->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitCondition(ConditionExpression::SPType spCondExpr)
{
  spCondExpr->GetReferenceExpression()->Visit(this);
  spCondExpr->GetTestExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  spTernExpr->GetReferenceExpression()->Visit(this);
  spTernExpr->GetTestExpression()->Visit(this);
  spTernExpr->GetTrueExpression()->Visit(this);
  spTernExpr->GetFalseExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  spIfElseExpr->GetReferenceExpression()->Visit(this);
  spIfElseExpr->GetTestExpression()->Visit(this);
  spIfElseExpr->GetThenExpression()->Visit(this);
  if (spIfElseExpr->GetElseExpression() != nullptr)
    spIfElseExpr->GetElseExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  spWhileExpr->GetReferenceExpression()->Visit(this);
  spWhileExpr->GetTestExpression()->Visit(this);
  spWhileExpr->GetBodyExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  spAssignExpr->GetDestinationExpression()->Visit(this);
  spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  spUnOpExpr->GetExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  spBinOpExpr->GetLeftExpression()->Visit(this);
  spBinOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitBitVector(BitVectorExpression::SPType spIntExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  if (spMemExpr->GetBaseExpression() != nullptr)
    spMemExpr->GetBaseExpression()->Visit(this);
  spMemExpr->GetOffsetExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  return nullptr;
}

Expression::SPType CloneVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return spSysExpr->Clone();
}

Expression::SPType CloneVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  Expression::LSPType Exprs;

  for (auto spExpr : spBindExpr->GetBoundExpressions())
    Exprs.push_back(spExpr->Visit(this));

  return Expr::MakeBind(Exprs);
}

Expression::SPType CloneVisitor::VisitCondition(ConditionExpression::SPType spCondExpr)
{
  return Expr::MakeCond(
    spCondExpr->GetType(),
    spCondExpr->GetReferenceExpression()->Visit(this),
    spCondExpr->GetTestExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  return Expr::MakeTernaryCond(
    spTernExpr->GetType(),
    spTernExpr->GetReferenceExpression()->Visit(this),
    spTernExpr->GetTestExpression()->Visit(this),
    spTernExpr->GetTrueExpression()->Visit(this),
    spTernExpr->GetFalseExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  auto spElseExpr = spIfElseExpr->GetElseExpression() != nullptr ? spIfElseExpr->GetElseExpression()->Visit(this) : nullptr;
  return Expr::MakeIfElseCond(
    spIfElseExpr->GetType(),
    spIfElseExpr->GetReferenceExpression()->Visit(this),
    spIfElseExpr->GetTestExpression()->Visit(this),
    spIfElseExpr->GetThenExpression()->Visit(this),
    spElseExpr);
}

Expression::SPType CloneVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return Expr::MakeWhileCond(
    spWhileExpr->GetType(),
    spWhileExpr->GetReferenceExpression()->Visit(this),
    spWhileExpr->GetTestExpression()->Visit(this),
    spWhileExpr->GetBodyExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  return Expr::MakeAssign(
    spAssignExpr->GetDestinationExpression()->Visit(this),
    spAssignExpr->GetSourceExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  return Expr::MakeUnOp(
    static_cast<OperationExpression::Type>(spUnOpExpr->GetOperation()),
    spUnOpExpr->GetExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  return Expr::MakeBinOp(
    static_cast<OperationExpression::Type>(spBinOpExpr->GetOperation()),
    spBinOpExpr->GetLeftExpression()->Visit(this),
    spBinOpExpr->GetRightExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitBitVector(BitVectorExpression::SPType spIntExpr)
{
  return Expr::MakeBitVector(spIntExpr->GetInt());
}

Expression::SPType CloneVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  return Expr::MakeId(
    spIdExpr->GetId(),
    spIdExpr->GetCpuInformation());
}

Expression::SPType CloneVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return Expr::MakeVecId(
    spVecIdExpr->GetVector(),
    spVecIdExpr->GetCpuInformation());
}

Expression::SPType CloneVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return Expr::MakeTrack(
    spTrkExpr->GetTrackedExpression(),
    spTrkExpr->GetTrackAddress(),
    spTrkExpr->GetTrackPosition()
    );
}

Expression::SPType CloneVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  return Expr::MakeVar(
    spVarExpr->GetName(),
    spVarExpr->GetAction(),
    spVarExpr->GetBitSize());
}

Expression::SPType CloneVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  auto spBaseExpr = spMemExpr->GetBaseExpression() ? spMemExpr->GetBaseExpression()->Visit(this) : nullptr;

  return Expr::MakeMem(
    spMemExpr->GetAccessSizeInBit(),
    spBaseExpr,
    spMemExpr->GetOffsetExpression()->Visit(this),
    spMemExpr->IsDereferencable());
}

Expression::SPType CloneVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  return Expr::MakeSym(
    spSymExpr->GetType(),
    spSymExpr->GetValue(),
    spSymExpr->GetAddress());
}

Expression::SPType FilterVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  _Evaluate(spSysExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  _Evaluate(spBindExpr);
  if (_IsDone())
    return nullptr;

  for (auto pExpr : spBindExpr->GetBoundExpressions())
  {
    pExpr->Visit(this);

    if (_IsDone())
      return nullptr;
  }

  return nullptr;
}

Expression::SPType FilterVisitor::VisitCondition(ConditionExpression::SPType spCondExpr)
{
  _Evaluate(spCondExpr);
  if (_IsDone())
    return nullptr;

  spCondExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spCondExpr->GetTestExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  _Evaluate(spTernExpr);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetTrueExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetFalseExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  _Evaluate(spIfElseExpr);
  if (_IsDone())
    return nullptr;

  spIfElseExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;;

  spIfElseExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spIfElseExpr->GetThenExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  if (spIfElseExpr->GetElseExpression() != nullptr)
    spIfElseExpr->GetElseExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  _Evaluate(spWhileExpr);
  if (_IsDone())
    return nullptr;

  spWhileExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spWhileExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spWhileExpr->GetBodyExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  _Evaluate(spAssignExpr);
  if (_IsDone())
    return nullptr;

  spAssignExpr->GetDestinationExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  _Evaluate(spUnOpExpr);
  if (_IsDone())
    return nullptr;

  spUnOpExpr->GetExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  return nullptr;
}

Expression::SPType FilterVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  _Evaluate(spBinOpExpr);

  if (_IsDone())
    return nullptr;
  spBinOpExpr->GetLeftExpression()->Visit(this);

  if (_IsDone())
    return nullptr;
  spBinOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitBitVector(BitVectorExpression::SPType spIntExpr)
{
  _Evaluate(spIntExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  _Evaluate(spIdExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  _Evaluate(spVecIdExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  _Evaluate(spTrkExpr);
  if (_IsDone())
    return nullptr;

  spTrkExpr->GetTrackedExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  _Evaluate(spVarExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  _Evaluate(spMemExpr);
  if (_IsDone())
    return nullptr;

  if (spMemExpr->GetBaseExpression() != nullptr)
    spMemExpr->GetBaseExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spMemExpr->GetOffsetExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  if (m_ExprMatcher(spSymExpr))
    m_MatchedExprs.push_back(spSymExpr);
  if (_IsDone())
    return nullptr;

  spSymExpr->GetExpression()->Visit(this);
  return nullptr;
}

void FilterVisitor::_Evaluate(Expression::SPType spExpr)
{
  _AddExpression(m_ExprMatcher(spExpr));
}

void FilterVisitor::_AddExpression(Expression::SPType spExpr)
{
  if (spExpr != nullptr)
    m_MatchedExprs.push_back(spExpr);
}

bool FilterVisitor::_IsDone(void) const
{
  if (m_NbrOfResult == 0)
    return false;
  return m_MatchedExprs.size() >= m_NbrOfResult;
}

TrackVisitor::TrackVisitor(Address const& rCurAddr, Track::Context& rCtxt)
: m_CurAddr(rCurAddr), m_rCtxt(rCtxt), m_spTrackedExpr(nullptr), m_IsAssigned(false)
{
}

Expression::SPType TrackVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  // NOTE: We must to visit the right side first, since the left side could
  // modify the Track context
  auto pTrkSrcExpr = spAssignExpr->GetSourceExpression()->Visit(this);

  m_IsAssigned = true;
  auto spTrkDstExpr = spAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  return Expr::MakeAssign(spTrkDstExpr, pTrkSrcExpr);
}

Expression::SPType TrackVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  Address TrackedAddress;

  if (m_IsAssigned)
  {
    TrackedAddress = m_CurAddr;
    m_rCtxt.TrackId(spIdExpr->GetId(), m_CurAddr);
  }
  else
  {
    if (!m_rCtxt.GetTrackAddress(spIdExpr->GetId(), TrackedAddress)
      || m_CurAddr == TrackedAddress)
      return CloneVisitor::VisitIdentifier(spIdExpr);
  }

  return Expr::MakeTrack(spIdExpr, TrackedAddress, 0);
}

Expression::SPType TrackVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
}

Expression::SPType BackTrackVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  m_IsAssigned = true;
  spAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  // TODO: can we have 2 OpAff in the same expression?
  spAssignExpr->GetSourceExpression()->Visit(this);
  m_TrackSource = false;

  return nullptr;
}

Expression::SPType BackTrackVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
}

EvaluateVisitor::EvaluateVisitor(Document const& rDoc, Address const& rCurAddr, u8 Mode, bool EvalMemRef)
: m_rDoc(rDoc), m_rCurAddr(rCurAddr), m_Mode(Mode)
, m_IsSymbolic(false), m_IsRelative(false), m_IsMemoryReference(false)
, m_EvalMemRef(EvalMemRef), m_spResExpr(nullptr)
{
}

Expression::SPType EvaluateVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitCondition(ConditionExpression::SPType spCondExpr)
{
  auto spBvRefExpr = expr_cast<BitVectorExpression>(spCondExpr->GetReferenceExpression()->Visit(this));
  auto spBvTestExpr = expr_cast<BitVectorExpression>(spCondExpr->GetTestExpression()->Visit(this));

  if (spBvRefExpr == nullptr || spBvTestExpr == nullptr)
  {
    m_IsSymbolic = true;
    return nullptr;
  }

  bool Res;
  if (!_EvaluateCondition(spCondExpr->GetCondition(), spBvRefExpr, spBvTestExpr, Res))
  {
    return nullptr;
  }

  m_spResExpr = Expr::MakeBoolean(Res);
  return spCondExpr;
}

Expression::SPType EvaluateVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  m_spResExpr = spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExpr = expr_cast<BitVectorExpression>(spUnOpExpr->GetExpression()->Visit(this));

  if (spExpr == nullptr)
    return nullptr;

  u32 Bit = spExpr->GetBitSize();
  auto Value = spExpr->GetInt();
  BitVector Result;

  switch (spUnOpExpr->GetOperation())
  {
  case OperationExpression::OpNot:
    Result = ~Value;
    break;

  case OperationExpression::OpNeg:
    Result = -Value;
    break;

  case OperationExpression::OpSwap:
    Result = Value.Swap();
    break;

  case OperationExpression::OpBsf:
    Result = Value.Bsf();
    break;

  case OperationExpression::OpBsr:
    Result = Value.Bsr();
    break;

  default:
    return nullptr;
  }

  m_spResExpr = Expr::MakeBitVector(Result);
  return m_spResExpr;
}

Expression::SPType EvaluateVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLExpr = expr_cast<BitVectorExpression>(spBinOpExpr->GetLeftExpression()->Visit(this));
  auto spRExpr = expr_cast<BitVectorExpression>(spBinOpExpr->GetRightExpression()->Visit(this));

  if (spLExpr == nullptr || spRExpr == nullptr)
    return nullptr;

  auto Left = spLExpr->GetInt();
  auto Right = spRExpr->GetInt();
  BitVector Result;

  switch (spBinOpExpr->GetOperation())
  {
  case OperationExpression::OpAdd:
    Result = Left + Right;
    break;

  case OperationExpression::OpSub:
    Result = Left - Right;
    break;

  case OperationExpression::OpMul:
    Result = Left * Right;
    break;

  case OperationExpression::OpUDiv:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UDiv(Right);
    break;

  case OperationExpression::OpSDiv:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SDiv(Right);
    break;

  case OperationExpression::OpUMod:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UMod(Right);
    break;

  case OperationExpression::OpSMod:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SMod(Right);
    break;

  case OperationExpression::OpAnd:
    Result = Left & Right;
    break;

  case OperationExpression::OpOr:
    Result = Left | Right;
    break;

  case OperationExpression::OpXor:
    Result = Left ^ Right;
    break;

  case OperationExpression::OpLls:
    Result = Left.Lls(Right);
    break;

  case OperationExpression::OpLrs:
    Result = Left.Lrs(Right);
    break;

  case OperationExpression::OpArs:
    Result = Left.Ars(Right);
    break;

  case OperationExpression::OpSext:
    Result = Left;
    Result.SignExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpZext:
    Result = Left;
    Result.ZeroExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpInsertBits:
    Result = ((Left << Right.Lsb()) & Right);
    break;

  case OperationExpression::OpExtractBits:
    Result = (Left & Right) >> Right.Lsb();
    break;

  case OperationExpression::OpBcast:
    Result = Left;
    Result.BitCast(Right.ConvertTo<u16>());
    break;

  default:
    return nullptr;
  }

  m_spResExpr = Expr::MakeBitVector(Result);
  return m_spResExpr;
}

Expression::SPType EvaluateVisitor::VisitBitVector(BitVectorExpression::SPType spBvExpr)
{
  m_spResExpr = spBvExpr;
  return m_spResExpr;
}

Expression::SPType EvaluateVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  auto const pCpuInfo = spIdExpr->GetCpuInformation();

  auto const PcBaseId = pCpuInfo->GetRegisterByType(CpuInformation::ProgramBaseRegister, m_Mode);
  auto const PcOffId  = pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_Mode);

  if (PcBaseId != 0 && PcBaseId == spIdExpr->GetId())
  {
    m_IsRelative = true;
    return Expr::MakeBitVector(m_rCurAddr.GetBaseSize(), m_rCurAddr.GetBase());
  }
  if (PcOffId != 0 && PcOffId == spIdExpr->GetId())
  {
    m_IsRelative = true;
    return Expr::MakeBitVector(m_rCurAddr.GetOffsetSize(), m_rCurAddr.GetOffset());
  }

  auto const itIdPair = m_Ids.find(spIdExpr->GetId());
  if (itIdPair != std::end(m_Ids))
  {
    return itIdPair->second->Visit(this);
  }

  m_IsSymbolic = true;
  return spIdExpr;
}

Expression::SPType EvaluateVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  Log::Write("core").Level(LogDebug) << "variable are not yet implemented in evaluate visitor" << LogEnd;
  return nullptr;
}

template<typename T>
bool ReadType(BinaryStream const& rBinStrm, TOffset FileOff, u64& rValue)
{
  T Value;
  if (!rBinStrm.Read(FileOff, Value))
    return false;
  rValue = Value;
  return true;
};

Expression::SPType EvaluateVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  u16 Base = 0;
  Expression::SPType spBaseConst;

  m_IsMemoryReference = true;

  if (spMemExpr->GetBaseExpression() != nullptr)
  {
    spBaseConst = expr_cast<BitVectorExpression>(spMemExpr->GetBaseExpression()->Visit(this));
    if (spBaseConst == nullptr)
    {
      m_IsSymbolic = true;
      return nullptr;
    }
  }

  auto spOffConst = expr_cast<BitVectorExpression>(spMemExpr->GetOffsetExpression()->Visit(this));
  if (spOffConst == nullptr)
  {
    m_IsSymbolic = true;
    return nullptr;
  }

  // Sometimes, we don't want to evaluate memory reference
  if (!m_EvalMemRef)
  {
    return Expr::MakeMem(spMemExpr->GetAccessSizeInBit(), spBaseConst, spOffConst, spMemExpr->IsDereferencable());
  }

  Address CurAddr(Base, spOffConst->GetInt().ConvertTo<u64>());

  TOffset FileOff;
  if (!m_rDoc.ConvertAddressToFileOffset(CurAddr, FileOff))
    return nullptr;
  auto const& rBinStrm = m_rDoc.GetBinaryStream();
  u64 Value;

  switch (spMemExpr->GetAccessSizeInBit())
  {
  case 8:
    if (!ReadType<u8>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 16:
    if (!ReadType<u16>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 32:
    if (!ReadType<u32>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 64:
    if (!ReadType<u64>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  default:
    return nullptr;
  }

  m_spResExpr = Expr::MakeBitVector(spMemExpr->GetAccessSizeInBit(), Value);
  return m_spResExpr;
}

Expression::SPType EvaluateVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  auto spRes = expr_cast<BitVectorExpression>(spSymExpr->GetExpression()->Visit(this));
  if (spRes == nullptr)
  {
    m_IsSymbolic = true;
    return nullptr;
  }
  m_spResExpr = spRes;
  return m_spResExpr;
}

void EvaluateVisitor::SetId(u32 Id, Expression::SPType spExpr)
{
  m_Ids[Id] = spExpr;
}

bool EvaluateVisitor::_EvaluateCondition(u8 CondOp, BitVectorExpression::SPType spConstRefExpr, BitVectorExpression::SPType spConstTestExpr, bool& rRes) const
{
  auto RefVal = spConstRefExpr->GetInt();
  auto TestVal = spConstTestExpr->GetInt();

  switch (CondOp)
  {
  case ConditionExpression::CondEq:
    rRes = RefVal.GetUnsignedValue() == TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondNe:
    rRes = RefVal.GetUnsignedValue() != TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUgt:
    rRes = RefVal.GetUnsignedValue() > TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUge:
    rRes = RefVal.GetUnsignedValue() >= TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUlt:
    rRes = RefVal.GetUnsignedValue() < TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUle:
    rRes = RefVal.GetUnsignedValue() <= TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondSgt:
    rRes = RefVal.GetSignedValue() > TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSge:
    rRes = RefVal.GetSignedValue() >= TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSlt:
    rRes = RefVal.GetSignedValue() < TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSle:
    rRes = RefVal.GetSignedValue() <= TestVal.GetSignedValue();
    break;

  default:
    Log::Write("core") << "unknown comparison" << LogEnd;
    return false;
  }

  return true;
}


SymbolicVisitor::SymbolicVisitor(Document const& rDoc, u8 Mode, bool EvalMemRef)
  : m_rDoc(rDoc), m_Mode(Mode)
  , m_IsSymbolic(false), m_IsRelative(false), m_IsMemoryReference(false), m_Update(true)
  , m_EvalMemRef(EvalMemRef)
  , m_CurPos()
{

}

Expression::SPType SymbolicVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  for (auto const& rspExpr : spBindExpr->GetBoundExpressions())
    rspExpr->Visit(this);
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitCondition(ConditionExpression::SPType spCondExpr)
{
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  bool OldUpdateState = m_Update;
  m_Update = true;
  auto spRefExpr = spTernExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spTernExpr->GetTestExpression()->Visit(this);
  m_Update = OldUpdateState;

  auto spConstRefExpr = expr_cast<BitVectorExpression>(spRefExpr);
  auto spConstTestExpr = expr_cast<BitVectorExpression>(spTestExpr);

  if (spConstRefExpr == nullptr || spConstTestExpr == nullptr)
  {
    return Expr::MakeTernaryCond(spTernExpr->GetType(),
      spConstRefExpr != nullptr ? spConstRefExpr : spRefExpr, spConstTestExpr != nullptr ? spConstTestExpr : spTestExpr,
      spTernExpr->GetTrueExpression()->Visit(this), spTernExpr->GetFalseExpression()->Visit(this));
  }

  bool Res;
  if (!_EvaluateCondition(spTernExpr->GetType(), spConstRefExpr, spConstTestExpr, Res))
    return nullptr;

  return Res ? spTernExpr->GetTrueExpression()->Visit(this) : spTernExpr->GetFalseExpression()->Visit(this);
}

Expression::SPType SymbolicVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  bool OldUpdateState = m_Update;
  m_Update = true;
  auto spRefExpr = spIfElseExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spIfElseExpr->GetTestExpression()->Visit(this);
  m_Update = OldUpdateState;

  // NOTE(wisk): If the condition use known values, we try to simplify it by evaluating it
  // and then apply the result
  auto spConstRefExpr = expr_cast<BitVectorExpression>(spRefExpr);
  auto spConstTestExpr = expr_cast<BitVectorExpression>(spTestExpr);
  if (spConstRefExpr == nullptr || spConstTestExpr == nullptr)
  {
    OldUpdateState = m_Update;
    m_Update = false;
    auto spOldCond = m_spCond;
    m_spCond = std::dynamic_pointer_cast<IfElseConditionExpression>(Expr::MakeIfElseCond(
      spIfElseExpr->GetType(), spRefExpr, spTestExpr,
      spIfElseExpr->GetThenExpression(), spIfElseExpr->GetElseExpression()));
    auto spVstThenExpr = spIfElseExpr->GetThenExpression()->Visit(this);
    auto spVstElseExpr = spIfElseExpr->GetElseExpression() != nullptr ? spIfElseExpr->GetThenExpression()->Visit(this) : nullptr;
    m_spCond = spOldCond;
    m_Update = OldUpdateState;
    return Expr::MakeIfElseCond(spIfElseExpr->GetType(),
      spConstRefExpr != nullptr ? spConstRefExpr : spRefExpr, spConstTestExpr != nullptr ? spConstTestExpr : spTestExpr,
      spVstThenExpr, spVstElseExpr);
  }

  bool Res;
  if (!_EvaluateCondition(spIfElseExpr->GetType(), spConstRefExpr, spConstTestExpr, Res))
    return nullptr;

  return Res ? spIfElseExpr->GetThenExpression()->Visit(this) : spIfElseExpr->GetElseExpression()->Visit(this);
}

Expression::SPType SymbolicVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  m_IsSymbolic = false;
  bool OldUpdate = m_Update;
  m_Update = true;
  auto spSrcExpr = spAssignExpr->GetSourceExpression()->Visit(this);
  m_Update = OldUpdate;
  ++m_CurPos;

  if (spSrcExpr == nullptr)
    return nullptr;

  auto spDstExpr = spAssignExpr->GetDestinationExpression();
  // NOTE(wisk): If the destination is an identifier, we don't want to transform it
  // to a bitvector if it's present on the symbolic context (e.g. pc register)
  if (auto spDstIdExpr = expr_cast<IdentifierExpression>(spDstExpr))
  {
    m_Update = false;
  }
  // NOTE(wisk): We can't update variable as destination
  else if (auto spDstVarExpr = expr_cast<VariableExpression>(spDstExpr))
  {
    m_Update = false;
  }
  auto spDstExprVst = spDstExpr->Visit(this);
  m_Update = OldUpdate;

  if (spDstExpr == nullptr || spDstExprVst == nullptr)
    return nullptr;

  // If we have a conditional expression, we need to transform the source to a ternary expression
  // We allow this operation even if update flag is false since we keep the conditional part
  if (m_spCond != nullptr)
  {
    m_Update = true;
    auto spTernExpr = Expr::MakeTernaryCond(
      m_spCond->GetType(), m_spCond->GetReferenceExpression(), m_spCond->GetTestExpression(),
      spSrcExpr, spDstExpr->Visit(this));
    spSrcExpr = spTernExpr;
  }

  if (m_Update)
  {
    for (auto const& rSymPair : m_SymCtxt)
    {
      auto spCurExpr = GetExpression(rSymPair.first);
      if (spCurExpr == nullptr)
        continue;

      if ((spCurExpr->Compare(spDstExpr) == Expression::CmpIdentical) || (spCurExpr->Compare(spDstExprVst) == Expression::CmpIdentical))
      {
        m_SymCtxt.erase(rSymPair.first);
        break;
      }
    }

    ExpressionRewriter ER(spSrcExpr);
    ER.Execute();

    m_SymCtxt[spDstExprVst] = spSrcExpr;
  }

  m_Update = OldUpdate;
  return Expr::MakeAssign(spDstExprVst, spSrcExpr);
}

Expression::SPType SymbolicVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExprVst = spUnOpExpr->GetExpression()->Visit(this);

  if (spExprVst == nullptr)
    return nullptr;

  auto spExpr = expr_cast<BitVectorExpression>(spExprVst);

  if (spExpr == nullptr)
    return Expr::MakeUnOp(static_cast<OperationExpression::Type>(spUnOpExpr->GetOperation()), spExprVst);

  u32 Bit = spExpr->GetBitSize();
  auto Value = spExpr->GetInt();
  BitVector Result;

  switch (spUnOpExpr->GetOperation())
  {
  case OperationExpression::OpNot:
    Result = ~Value;
    break;

  case OperationExpression::OpNeg:
    Result = -Value;
    break;

  case OperationExpression::OpSwap:
    Result = Value.Swap();
    break;

  case OperationExpression::OpBsf:
    Result = Value.Bsf();
    break;

  case OperationExpression::OpBsr:
    Result = Value.Bsr();
    break;

  default:
    return nullptr;
  }

  return Expr::MakeBitVector(Result);
}

Expression::SPType SymbolicVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLExprVst = spBinOpExpr->GetLeftExpression()->Visit(this);
  auto spRExprVst = spBinOpExpr->GetRightExpression()->Visit(this);

  if (spLExprVst == nullptr || spRExprVst == nullptr)
    return nullptr;

  auto spLExpr = expr_cast<BitVectorExpression>(spLExprVst);
  auto spRExpr = expr_cast<BitVectorExpression>(spRExprVst);

  if (spLExpr == nullptr || spRExpr == nullptr)
    return Expr::MakeBinOp(static_cast<OperationExpression::Type>(spBinOpExpr->GetOperation()),
    /**/spLExprVst, spRExprVst);

  auto Left = spLExpr->GetInt();
  auto Right = spRExpr->GetInt();
  BitVector Result;

  switch (spBinOpExpr->GetOperation())
  {
  case OperationExpression::OpAdd:
    Result = Left + Right;
    break;

  case OperationExpression::OpSub:
    Result = Left - Right;
    break;

  case OperationExpression::OpMul:
    Result = Left * Right;
    break;

  case OperationExpression::OpUDiv:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UDiv(Right);
    break;

  case OperationExpression::OpSDiv:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SDiv(Right);
    break;

  case OperationExpression::OpUMod:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UMod(Right);
    break;

  case OperationExpression::OpSMod:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SMod(Right);
    break;

  case OperationExpression::OpAnd:
    Result = Left & Right;
    break;

  case OperationExpression::OpOr:
    Result = Left | Right;
    break;

  case OperationExpression::OpXor:
    Result = Left ^ Right;
    break;

  case OperationExpression::OpLls:
    Result = Left.Lls(Right);
    break;

  case OperationExpression::OpLrs:
    Result = Left.Lrs(Right);
    break;

  case OperationExpression::OpArs:
    Result = Left.Ars(Right);
    break;

  case OperationExpression::OpSext:
    Result = Left;
    Result.SignExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpZext:
    Result = Left;
    Result.ZeroExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpInsertBits:
    Result = ((Left << Right.Lsb()) & Right);
    break;

  case OperationExpression::OpExtractBits:
    Result = (Left & Right) >> Right.Lsb();
    break;

  case OperationExpression::OpBcast:
    Result = Left;
    Result.BitCast(Right.ConvertTo<u16>());
    break;

  default:
    return nullptr;
  }

  return Expr::MakeBitVector(Result);
}

Expression::SPType SymbolicVisitor::VisitBitVector(BitVectorExpression::SPType spIntExpr)
{
  return spIntExpr;
}

Expression::SPType SymbolicVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spCurExpr = GetExpression(rSymPair.first);

    if (auto spSymIdExpr = expr_cast<IdentifierExpression>(spCurExpr))
    {
      if (spSymIdExpr->GetId() == spIdExpr->GetId())
        if (m_Update)
          return rSymPair.second;
        else
          return rSymPair.first;
    }
  }

  if (!m_Update)
    return spIdExpr;

  m_IsSymbolic = true;
  auto spTrkId = Expr::MakeTrack(spIdExpr, m_CurAddr, m_CurPos);
  auto spSymId = Expr::MakeSym(SymbolicExpression::Undefined, "sym_vst", m_CurAddr, spIdExpr);
  m_SymCtxt[spTrkId] = spSymId;
  return spSymId;
}

Expression::SPType SymbolicVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
}

Expression::SPType SymbolicVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  auto itVar = m_VarPool.find(spVarExpr->GetName());
  if (itVar == std::end(m_VarPool))
  {
    if (spVarExpr->GetAction() != VariableExpression::Alloc)
    {
      Log::Write("core").Level(LogError) << "invalid var expr action while evaluation" << LogEnd;
      return nullptr;
    }
    m_VarPool.insert(spVarExpr->GetName());
  }
  else
  {
    switch (spVarExpr->GetAction())
    {
    default:
      break;

    case VariableExpression::Alloc:
      Log::Write("core").Level(LogError) << "try to allocate an already allocated var while evaluation" << LogEnd;
      break;

    case VariableExpression::Use:
    {
      if (!m_Update)
        return spVarExpr;

      for (auto const& rSymPair : m_SymCtxt)
      {
        auto spCurExpr = GetExpression(rSymPair.first);
        if (auto spSymVarExpr = expr_cast<VariableExpression>(spCurExpr))
        {
          if (spSymVarExpr->GetName() == spVarExpr->GetName())
            return rSymPair.second;
        }
      }
      break;
    }

    case VariableExpression::Free:
      m_VarPool.erase(spVarExpr->GetName());
      for (auto const& rSymPair : m_SymCtxt)
      {
        auto spCurExpr = GetExpression(rSymPair.first);
        if (auto spSymVarExpr = expr_cast<VariableExpression>(spCurExpr))
        {
          if (spSymVarExpr->GetName() == spVarExpr->GetName())
          {
            m_SymCtxt.erase(rSymPair.first);
            break;
          }
        }
      }
      break;
    }
  }
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  Expression::SPType spBaseExpr;
  Expression::SPType spOffExpr;

  m_IsMemoryReference = true;

  bool OldUpdateState = m_Update;
  m_Update = true;
  if (spMemExpr->GetBaseExpression() != nullptr)
  {
    spBaseExpr = spMemExpr->GetBaseExpression()->Visit(this);
  }

  spOffExpr = spMemExpr->GetOffsetExpression()->Visit(this);
  m_Update = OldUpdateState;

  // Sometimes, we don't want to evaluate memory reference or we can't
  auto spOffConstExpr = expr_cast<BitVectorExpression>(spOffExpr);
  if (spOffConstExpr == nullptr)
  {
    auto spMemExprVst = Expr::MakeMem(spMemExpr->GetAccessSizeInBit(), spBaseExpr, spOffExpr, spMemExpr->IsDereferencable());
    if (!m_Update)
      return spMemExprVst;

    auto spFoundExpr = FindExpression(spMemExprVst);
    if (spFoundExpr == nullptr)
      return spMemExprVst;
    return spFoundExpr;
  }

  auto Offset = spOffConstExpr->GetInt().ConvertTo<TOffset>();

  TBase Base = 0;
  if (auto spBaseConstExpr = expr_cast<BitVectorExpression>(spBaseExpr))
    Base = spBaseConstExpr->GetInt().ConvertTo<TBase>();

  Address CurAddr(Base, Offset);

  auto Lbl = m_rDoc.GetLabelFromAddress(CurAddr);
  if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
  {
    auto LblAddr = m_rDoc.GetAddressFromLabelName(Lbl.GetName());
    return Expr::MakeSym(SymbolicExpression::ExternalFunction, Lbl.GetName(), LblAddr);
  }

  if (!m_EvalMemRef)
  {
    auto spMemExprVst = Expr::MakeMem(spMemExpr->GetAccessSizeInBit(), spBaseExpr, spOffExpr, spMemExpr->IsDereferencable());
    if (!m_Update)
      return spMemExprVst;

    auto spFoundExpr = FindExpression(spMemExprVst);
    if (spFoundExpr == nullptr)
      return spMemExprVst;
    return spFoundExpr;
  }

  TOffset FileOff;
  if (!m_rDoc.ConvertAddressToFileOffset(CurAddr, FileOff))
    return nullptr;
  auto const& rBinStrm = m_rDoc.GetBinaryStream();
  u64 Value;

  switch (spMemExpr->GetAccessSizeInBit())
  {
  case 8:
    if (!ReadType<u8>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 16:
    if (!ReadType<u16>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 32:
    if (!ReadType<u32>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 64:
    if (!ReadType<u64>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  // LATER(wisk): rely on BitVector to support any read size

  default:
    return nullptr;
  }

  return Expr::MakeBitVector(spMemExpr->GetAccessSizeInBit(), Value);
}

Expression::SPType SymbolicVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

SymbolicVisitor SymbolicVisitor::Fork(void) const
{
  SymbolicVisitor Forked(m_rDoc, m_Mode, m_EvalMemRef);

  for (auto const& rSymCtxtPair : m_SymCtxt)
  {
    Forked.m_SymCtxt[rSymCtxtPair.first] = rSymCtxtPair.second->Clone();
  }

  for (auto const& rSymCond : m_SymCond)
    Forked.m_SymCond.push_back(rSymCond->Clone());

  Forked.m_VarPool = m_VarPool;

  Forked.m_IsSymbolic         = Forked.m_IsSymbolic;
  Forked.m_IsRelative         = Forked.m_IsRelative;
  Forked.m_IsMemoryReference  = Forked.m_IsMemoryReference;
  Forked.m_Update             = m_Update;

  Forked.m_CurAddr = m_CurAddr;
  Forked.m_CurPos  = m_CurPos;

  return Forked;
}

Expression::VSPType SymbolicVisitor::GetExpressions(void) const
{
  Expression::VSPType Res;
  for (auto const& rSymPair : m_SymCtxt)
    Res.push_back(rSymPair.second);
  return Res;
}

std::string SymbolicVisitor::ToString(void) const
{
  static const std::string s_Sep(80, '_');
  std::ostringstream oss;
  oss << "condition:" << std::endl;
  for (auto spCond : m_SymCond)
  {
    oss << spCond->ToString() << std::endl;
  }
  oss << std::endl;

  oss << "context:" << std::endl;
  for (auto const& rSymPair : m_SymCtxt)
  {
    oss << rSymPair.first->ToString() << " = ";
    if (rSymPair.second == nullptr)
      oss << "<null>";
    else
      oss << rSymPair.second->ToString();
    oss << std::endl << s_Sep << std::endl;
  }
  return oss.str();
}

bool SymbolicVisitor::BindExpression(Expression::SPType spKeyExpr, Expression::SPType spValueExpr, bool Propagate)
{
  m_SymCtxt[spKeyExpr] = spValueExpr;

  if (!Propagate)
    return true;

  bool Res = false;
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spClonedExpr = rSymPair.second->Clone();

    if (!spClonedExpr->UpdateChild(spKeyExpr, spValueExpr))
      continue;

    Res = true;
    m_SymCtxt[rSymPair.first] = spClonedExpr;
  }

  return Res;
}

bool SymbolicVisitor::UpdateAddress(Architecture& rArch, Address const& rAddr)
{
  m_CurAddr = rAddr;
  m_CurPos = 0;

  auto spInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(rAddr));
  if (spInsn == nullptr)
  {
    return false;
  }

  // Get PC register
  auto const pCpuInfo = rArch.GetCpuInformation();
  auto PcId = pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, spInsn->GetMode());
  if (PcId == 0)
    return false;

  // If the user makes assumption, we need to store the condition
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spDstIdExpr = expr_cast<IdentifierExpression>(rSymPair.first);
    if (spDstIdExpr == nullptr)
      continue;
    if (spDstIdExpr->GetId() != PcId)
      continue;
    auto spTernExpr = expr_cast<TernaryConditionExpression>(rSymPair.second);
    if (spTernExpr == nullptr)
      continue;

    // NOTE(wisk): Usually conditional branch where ``true'' part is the new address
    // At this time, we don't support conditional branch with known register
    auto spTrueVal = expr_cast<BitVectorExpression>(spTernExpr->GetTrueExpression());
    if (spTrueVal == nullptr)
      continue;
    auto spFalseId = expr_cast<IdentifierExpression>(spTernExpr->GetFalseExpression());
    if (spFalseId != nullptr)
      if (spFalseId->GetId() != PcId)
        continue;
    auto spFalseVal = expr_cast<BitVectorExpression>(spTernExpr->GetFalseExpression());
    if (spFalseVal == nullptr)
      continue;

    auto Cond = spTrueVal->GetInt().ConvertTo<u64>() == m_CurAddr.GetOffset() ? spTernExpr->GetCondition() : spTernExpr->GetOppositeCondition();
    auto spCondExpr = Expr::MakeCond(Cond, spTernExpr->GetReferenceExpression(), spTernExpr->GetTestExpression());
    m_SymCond.push_back(spCondExpr);
    break;
  }

  // Update PC address
  auto ExecAddr = rArch.CurrentAddress(rAddr, *spInsn);
  Expression::VSPType SetCurAddrExprs;

  if (!rArch.EmitSetExecutionAddress(SetCurAddrExprs, ExecAddr, m_Mode))
  {
    return false;
  }

  for (auto const& rspExpr : SetCurAddrExprs)
    rspExpr->Visit(this);

  return true;
}

bool SymbolicVisitor::UpdateExpression(Expression::SPType spKeyExpr, SymbolicVisitor::Updater updt)
{
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spCurExpr = GetExpression(rSymPair.first);
    if (spKeyExpr->Compare(spCurExpr) == Expression::CmpIdentical)
    {
      auto spClonedExpr = rSymPair.second->Clone();
      if (!updt(spClonedExpr))
        return false;
      m_SymCtxt[rSymPair.first] = spClonedExpr;
      return true;
    }
  }
  return false;
}

bool SymbolicVisitor::FindAllPaths(int& rNumOfPathFound, Architecture& rArch, SymbolicVisitor::DestinationPathCallbackType DstPathCb)
{
  rNumOfPathFound = 0;
  auto const pCpuInfo = rArch.GetCpuInformation();
  if (pCpuInfo == nullptr)
    return false;

  auto PcId = pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister,  m_rDoc.GetMode(m_CurAddr));
  if (PcId == 0)
    return false;

  auto spPcExpr = FindExpression(Expr::MakeId(PcId, pCpuInfo));
  if (spPcExpr == nullptr)
    return false;

  // Is it a simple branch to a register (converted to a BitVector)?
  auto spBvDst = expr_cast<BitVectorExpression>(spPcExpr);
  if (spBvDst != nullptr)
  {
    ++rNumOfPathFound;
    DstPathCb(spBvDst, {});
    return true;
  }

  // Is it a call to an API?
  auto spSymDst = expr_cast<SymbolicExpression>(spPcExpr);
  if (spSymDst != nullptr)
  {
    ++rNumOfPathFound;
    DstPathCb(spSymDst, {});
    return true;
  }

  // Is it a conditional branch?
  auto spTernDst = expr_cast<TernaryConditionExpression>(spPcExpr);
  if (spTernDst != nullptr)
  {
    rNumOfPathFound += 2;
    DstPathCb(spTernDst->GetTrueExpression(),
    {
      Expr::MakeCond(
      spTernDst->GetCondition(),
      spTernDst->GetReferenceExpression(), spTernDst->GetTestExpression())
    });

    DstPathCb(spTernDst->GetFalseExpression(),
    {
      Expr::MakeCond(
      spTernDst->GetOppositeCondition(),
      spTernDst->GetReferenceExpression(), spTernDst->GetTestExpression())
    });

    return true;
  }

  // Is it a branch table (switch)
  auto spMemDst = expr_cast<MemoryExpression>(spPcExpr);
  if (spMemDst != nullptr)
  {
    using namespace Pattern;

    // TODO(wisk): this pattern is not fully generic
    ExpressionFilter ExprFlt_JmpTbl(Memory(
      ADD(
      /**/MUL(Any("idx"), Any("ptr_sz")),
      /**/Any("tbl_addr"))));

    if (!ExprFlt_JmpTbl.Execute(spMemDst))
      return false;
    auto spIdxExpr     = ExprFlt_JmpTbl.GetExpression("idx");
    auto spPtrSzExpr   = expr_cast<BitVectorExpression>(ExprFlt_JmpTbl.GetExpression("ptr_sz"));
    auto spTblAddrExpr = expr_cast<BitVectorExpression>(ExprFlt_JmpTbl.GetExpression("tbl_addr"));

    if (spIdxExpr == nullptr || spPtrSzExpr == nullptr || spTblAddrExpr == nullptr)
      return false;

    Log::Write("core") << "idx: " << spIdxExpr->ToString() << ", ptr_sz: " << spPtrSzExpr->ToString() << ", tbl_addr: " << spTblAddrExpr->ToString() << LogEnd;

    FilterVisitor FindId([](Expression::SPType spExpr) -> Expression::SPType
    {
      auto spIdExpr = expr_cast<IdentifierExpression>(spExpr);
      if (spIdExpr == nullptr)
        return nullptr;
      return spIdExpr;
    });

    // TODO(wisk): handle more conditions if possible
    spIdxExpr->Visit(&FindId);
    auto Res = FindId.GetMatchedExpressions();
    if (Res.size() != 1)
      return false;
    auto spIdxRegExpr = expr_cast<IdentifierExpression>(Res.front());

    // In this case, we actually need the opposite operation to retrieve the index
    Expression::SPType spOppIdxExpr = nullptr;
    auto spBinOpIdxExpr = expr_cast<BinaryOperationExpression>(spIdxExpr);
    if (spBinOpIdxExpr != nullptr)
    {
      auto OppOp = spBinOpIdxExpr->GetOppositeOperation();
      spOppIdxExpr = Expr::MakeBinOp(static_cast<OperationExpression::Type>(OppOp), spBinOpIdxExpr->GetLeftExpression(), spBinOpIdxExpr->GetRightExpression());
      Log::Write("core").Level(LogDebug) << "take opposite operation: " << spOppIdxExpr->ToString() << LogEnd;
    }

    // Try to figure out the first base
    BitVector Idx(spIdxRegExpr->GetBitSize(), 0);
    EvaluateVisitor EvalIdx(m_rDoc, m_CurAddr, m_Mode, true);
    EvalIdx.SetId(spIdxRegExpr->GetId(), Expr::MakeBitVector(Idx));
    spOppIdxExpr->Visit(&EvalIdx);
    if (!EvalIdx.IsSymbolic())
    {
      auto spBvIdxExpr = expr_cast<BitVectorExpression>(EvalIdx.GetResultExpression());
      if (spBvIdxExpr != nullptr)
        Idx = spBvIdxExpr->GetInt();
    }

    // LATER(wisk): at this time, we only support one condition...
    auto spCurCond = m_SymCond.front();
    bool FoundPath = false;
    while (true)
    {
      EvaluateVisitor EvalCondVst(m_rDoc, m_CurAddr, m_Mode, true);
      EvaluateVisitor EvalDstVst(m_rDoc, m_CurAddr, m_Mode, true);

      EvalCondVst.SetId(spIdxRegExpr->GetId(), Expr::MakeBitVector(Idx));
      spCurCond->Visit(&EvalCondVst);
      if (EvalCondVst.IsSymbolic())
        break;
      auto spCondResExpr = expr_cast<BitVectorExpression>(EvalCondVst.GetResultExpression());
      if (spCondResExpr == nullptr)
        break;
      if (spCondResExpr->GetBitSize() != 1)
        break;
      if (spCondResExpr->GetInt().ConvertTo<u8>() == 0)
        break;

      EvalDstVst.SetId(spIdxRegExpr->GetId(), Expr::MakeBitVector(Idx));
      spMemDst->Visit(&EvalDstVst);
      if (EvalDstVst.IsSymbolic())
        break;
      auto spDstResExpr = expr_cast<BitVectorExpression>(EvalDstVst.GetResultExpression());
      if (spDstResExpr == nullptr)
        break;

      // TODO(wisk): this address is not fully generic since it doesn't update the base address
      Address DstAddr = m_CurAddr;
      DstAddr.SetOffset(spDstResExpr->GetInt().ConvertTo<TOffset>());
      auto spAssumedExpr = Expr::MakeAssign(spIdxRegExpr, Expr::MakeBitVector(Idx));
      ++rNumOfPathFound;
      DstPathCb(spDstResExpr, { spAssumedExpr });

      ++Idx;
    }

    return true;
  }

  return true;
}

Expression::SPType SymbolicVisitor::GetExpression(Expression::SPType spExpr)
{
  if (auto spTrkExpr = expr_cast<TrackExpression>(spExpr))
    return spTrkExpr->GetTrackedExpression();
  if (auto spSymExpr = expr_cast<SymbolicExpression>(spExpr))
    return spSymExpr->GetExpression();
  return spExpr;
}

Expression::SPType SymbolicVisitor::FindExpression(Expression::SPType spExpr)
{
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spCurExpr = GetExpression(rSymPair.first);
    if (spExpr->Compare(spCurExpr) == Expression::CmpIdentical)
      return rSymPair.second;
  }
  return nullptr;
}

bool SymbolicVisitor::_EvaluateCondition(u8 CondOp, BitVectorExpression::SPType spConstRefExpr, BitVectorExpression::SPType spConstTestExpr, bool& rRes) const
{
  auto RefVal = spConstRefExpr->GetInt();
  auto TestVal = spConstTestExpr->GetInt();

  switch (CondOp)
  {
  case ConditionExpression::CondEq:
    rRes = RefVal.GetUnsignedValue() == TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondNe:
    rRes = RefVal.GetUnsignedValue() != TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUgt:
    rRes = RefVal.GetUnsignedValue() > TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUge:
    rRes = RefVal.GetUnsignedValue() >= TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUlt:
    rRes = RefVal.GetUnsignedValue() < TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUle:
    rRes = RefVal.GetUnsignedValue() <= TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondSgt:
    rRes = RefVal.GetSignedValue() > TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSge:
    rRes = RefVal.GetSignedValue() >= TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSlt:
    rRes = RefVal.GetSignedValue() < TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSle:
    rRes = RefVal.GetSignedValue() <= TestVal.GetSignedValue();
    break;

  default:
    Log::Write("core") << "unknown comparison" << LogEnd;
    return false;
  }

  return true;
}


NormalizeIdentifier::NormalizeIdentifier(CpuInformation const& rCpuInfo, u8 Mode)
: m_rCpuInfo(rCpuInfo), m_Mode(Mode)
{
}

Expression::SPType NormalizeIdentifier::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  auto spNrmSrc = spAssignExpr->GetSourceExpression()->Visit(this);

  if (auto spDstId = expr_cast<IdentifierExpression>(spAssignExpr->GetDestinationExpression()))
  {
    u32 Id = spDstId->GetId();
    u32 NrmId = 0;
    u64 Mask = 0x0;

    if (m_rCpuInfo.NormalizeRegister(Id, m_Mode, NrmId, Mask))
    {
      auto const* pCpuInfo = spDstId->GetCpuInformation();
      u32 NrmIdBitSize = pCpuInfo->GetSizeOfRegisterInBit(NrmId);

      // ExtDstId = (ExtDstId & ~Mask) | InsertBits(bit_cast(ExtSrc, NrmIdBitSize))
      auto spExtDstId = Expr::MakeId(NrmId, pCpuInfo);
      auto spMask = Expr::MakeBitVector(NrmIdBitSize, Mask);
      auto spNotMask = Expr::MakeBitVector(NrmIdBitSize, ~Mask);
      auto spBitCastSrc = Expr::MakeBinOp(OperationExpression::OpBcast, spNrmSrc, Expr::MakeBitVector(NrmIdBitSize, NrmIdBitSize));
      auto spInsertBits = Expr::MakeBinOp(OperationExpression::OpInsertBits, spBitCastSrc, spMask);
      auto spClearBits = Expr::MakeBinOp(OperationExpression::OpAnd, spExtDstId, spNotMask);
      auto spExtSrc = Expr::MakeBinOp(OperationExpression::OpOr, spClearBits, spInsertBits);

      return Expr::MakeAssign(spExtDstId, spExtSrc);
    }
  }

  return Expr::MakeAssign(spAssignExpr->GetDestinationExpression(), spNrmSrc);
}

Expression::SPType NormalizeIdentifier::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  u32 Id = spIdExpr->GetId();
  u32 NrmId = 0;
  u64 Mask = 0x0;

  if (!m_rCpuInfo.NormalizeRegister(Id, m_Mode, NrmId, Mask))
    return spIdExpr;
  u32 IdBitSize = m_rCpuInfo.GetSizeOfRegisterInBit(Id);
  u32 NrmIdBitSize = m_rCpuInfo.GetSizeOfRegisterInBit(NrmId);

  auto spExtractBits = Expr::MakeBinOp(
    OperationExpression::OpExtractBits,
    Expr::MakeId(NrmId, spIdExpr->GetCpuInformation()),
    Expr::MakeBitVector(NrmIdBitSize, Mask));
  return Expr::MakeBinOp(OperationExpression::OpBcast,
    spExtractBits, Expr::MakeBitVector(IdBitSize, IdBitSize));
}

Expression::SPType IdentifierToVariable::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  auto pCpuInfo = spIdExpr->GetCpuInformation();
  auto Id = spIdExpr->GetId();
  m_UsedId.insert(Id);
  return Expr::MakeVar(pCpuInfo->ConvertIdentifierToName(Id), VariableExpression::Use);
}

Expression::SPType SimplifyVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  return nullptr;
}

Expression::SPType SimplifyVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  return nullptr;
}

MEDUSA_NAMESPACE_END
