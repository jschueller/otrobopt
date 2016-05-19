#!/usr/bin/env python

from __future__ import print_function
import openturns as ot
import otrobopt

thetaDist = ot.Normal(2.0, 0.1)
f_base = ot.NumericalMathFunction(['x', 'theta'], ['y'], ['x*theta'])
f = ot.NumericalMathFunction(f_base, [1], [1.0])

x = [1.0]

measures = [otrobopt.MeanMeasure(f, thetaDist),
            otrobopt.VarianceMeasure(f, thetaDist),
            otrobopt.WorstCaseMeasure(f, ot.Uniform(-1.0, 4.0)),
            otrobopt.WorstCaseMeasure(f, ot.Uniform(-1.0, 4.0), False),
            otrobopt.JointChanceMeasure(f, ot.Normal(1.0, 1.0), ot.GreaterOrEqual(), 0.95),
            otrobopt.IndividualChanceMeasure(f, ot.Normal(1.0, 1.0), ot.GreaterOrEqual(), [0.95]),
            otrobopt.MeanStandardDeviationTradeoffMeasure(f, thetaDist, [0.8]),
            otrobopt.QuantileMeasure(f, thetaDist, 0.99)
            ]
aggregated = otrobopt.AggregatedMeasure(measures)
measures.append(aggregated)

for measure in measures:
    print(measure, '(continuous)', measure(x))
    N = 10000
    experiment = ot.LHSExperiment(N)
    factory = otrobopt.MeasureFactory(experiment)
    discretizedMeasure = factory.build(measure)
    print(discretizedMeasure, '(discretized)', discretizedMeasure(x))
