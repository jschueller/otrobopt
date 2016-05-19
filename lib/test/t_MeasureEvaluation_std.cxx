#include <iostream>

#include "otrobopt/OTRobOpt.hxx"
#include "openturns/Normal.hxx"
#include "openturns/LHSExperiment.hxx"
#include "openturns/GreaterOrEqual.hxx"
#include "openturns/Uniform.hxx"

using namespace OT;
using namespace OTROBOPT;

int main(int argc, char **argv)
{
  Normal thetaDist(2.0, 0.1);
  Description input(2);
  input[0] = "x1";
  input[1] = "p1";
  NumericalMathFunction f_base(input, Description(1, "y1"), Description(1, "x1*p1"));
  NumericalMathFunction f(f_base, Indices(1 , 1), NumericalPoint(1, 1.0));

  NumericalPoint x(1, 1.0);

  Collection <MeasureEvaluation> measures;
  measures.add(MeanMeasure(f, thetaDist));
  measures.add(VarianceMeasure(f, thetaDist));
  measures.add(WorstCaseMeasure(f, Uniform(-1.0, 4.0)));
  measures.add(WorstCaseMeasure(f, Uniform(-1.0, 4.0), false));
  measures.add(JointChanceMeasure(f, Normal(1.0, 1.0), GreaterOrEqual(), 0.95));
  measures.add(IndividualChanceMeasure(f, Normal(1.0, 1.0), GreaterOrEqual(), NumericalPoint(1, 0.95)));
  measures.add(MeanStandardDeviationTradeoffMeasure(f, thetaDist, NumericalPoint(1, 0.8)));
  measures.add(QuantileMeasure(f, thetaDist, 0.99));

  AggregatedMeasure aggregated(measures);
  measures.add(aggregated);

  const UnsignedInteger size = measures.getSize();
  for(UnsignedInteger i = 0; i < size; ++ i)
  {
    MeasureEvaluation measure(measures[i]);
    std::cout << measure << "(continuous)"<<measure(x)<< std::endl;
    const UnsignedInteger N = 1000;
    LHSExperiment experiment(N);
    MeasureFactory factory(experiment);
    MeasureEvaluation discretizedMeasure(factory.build(measure));
    std::cout << discretizedMeasure << "(discretized)"<<discretizedMeasure(x)<< std::endl;
  }
}

