//                                               -*- C++ -*-
/**
 *  @brief JointChanceMeasure
 *
 *  Copyright 2005-2016 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */
#include "otrobopt/JointChanceMeasure.hxx"
#include <openturns/PersistentObjectFactory.hxx>
#include <openturns/GaussKronrod.hxx>
#include <openturns/IteratedQuadrature.hxx>

using namespace OT;

namespace OTROBOPT
{

CLASSNAMEINIT(JointChanceMeasure);

static Factory<JointChanceMeasure> RegisteredFactory;


/* Default constructor */
JointChanceMeasure::JointChanceMeasure()
  : MeasureFunctionImplementation()
  , alpha_(0.0)
{
  // Nothing to do
}

/* Parameter constructor */
JointChanceMeasure::JointChanceMeasure (const Distribution & distribution,
                                        const NumericalMathFunction & function,
                                        const NumericalScalar alpha)
  : MeasureFunctionImplementation(distribution, function)
  , alpha_(0.0)
{
  setAlpha(alpha);
}

/* Virtual constructor method */
JointChanceMeasure * JointChanceMeasure::clone() const
{
  return new JointChanceMeasure(*this);
}


class JointChanceMeasureParametricFunctionWrapper : public NumericalMathFunctionImplementation
{
public:
  JointChanceMeasureParametricFunctionWrapper(const NumericalPoint & x,
                            const NumericalMathFunction & function)
  : NumericalMathFunctionImplementation()
  , x_(x)
  , function_(function)
  {}

  virtual JointChanceMeasureParametricFunctionWrapper * clone() const
  {
    return new JointChanceMeasureParametricFunctionWrapper(*this);
  }

  NumericalPoint operator()(const NumericalPoint & theta) const
  {
    NumericalMathFunction function(function_);
    return function(x_, theta);
  }

  NumericalSample operator()(const NumericalSample & theta) const
  {
    const UnsignedInteger size = theta.getSize();
    const UnsignedInteger outputDimension = function_.getOutputDimension();
    NumericalSample outS(size, 1);
    for (UnsignedInteger i = 0; i < size; ++ i)
    {
      NumericalPoint outPi(operator()(theta[i]));
      outS[i][0] = 1.0;
      for (UnsignedInteger j = 0; j < outputDimension; ++ j)
      {
        outS[i][0] *= (outPi[j] > 0.0 ? 1.0 : 0.0);
      }
    }
    return outS;
  }

  UnsignedInteger getInputDimension() const
  {
    return function_.getInputDimension();
  }

  UnsignedInteger getOutputDimension() const
  {
    return 1;
  }

protected:
  NumericalPoint x_;
  NumericalMathFunction function_;
};


/* Evaluation */
NumericalPoint JointChanceMeasure::operator()(const NumericalPoint & inP) const
{
  NumericalMathFunction function(getFunction());
  NumericalPoint parameter(function.getParameter());
  const UnsignedInteger outputDimension = getFunction().getOutputDimension();
  NumericalPoint outP(1);
  if (getDistribution().isContinuous())
  {
    GaussKronrod gkr;
    gkr.setRule(GaussKronrodRule::G1K3);
    IteratedQuadrature algo(gkr);
    Pointer<NumericalMathFunctionImplementation> p_wrapper(new JointChanceMeasureParametricFunctionWrapper(inP, function));
    NumericalMathFunction G(p_wrapper);
    outP = algo.integrate(G, getDistribution().getRange()) * 1.0 / getDistribution().getRange().getVolume();
  }
  else
  {
    NumericalSample support(getDistribution().getSupport());
    const UnsignedInteger size = support.getSize();
    for (UnsignedInteger i = 0; i < size; ++ i)
    {
      NumericalPoint outPi(function(inP, support[i]));
      NumericalScalar outV = 1.0;
      for (UnsignedInteger j = 0; j < outputDimension; ++ j)
      {
        outV *= outPi[j] > 0.0 ? 1.0 : 0.0;
      }
      outP[0] += outV / size;
    }
  }
  outP[0] = alpha_ - outP[0];
  function.setParameter(parameter);
  return outP;
}

UnsignedInteger JointChanceMeasure::getOutputDimension() const
{
  return 1;
}

/* Alpha coefficient accessor */
void JointChanceMeasure::setAlpha(const NumericalScalar alpha)
{
  if (!(alpha >= 0.0) || !(alpha <= 1.0))
    throw InvalidArgumentException(HERE) << "Alpha should be in (0, 1)";
  alpha_ = alpha;
}

NumericalScalar JointChanceMeasure::getAlpha() const
{
  return alpha_;
}

/* String converter */
String JointChanceMeasure::__repr__() const
{
  OSS oss;
  oss << "class=" << JointChanceMeasure::GetClassName()
      << " alpha=" << alpha_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void JointChanceMeasure::save(Advocate & adv) const
{
  MeasureFunctionImplementation::save(adv);
  adv.saveAttribute("alpha_", alpha_);
}

/* Method load() reloads the object from the StorageManager */
void JointChanceMeasure::load(Advocate & adv)
{
  MeasureFunctionImplementation::load(adv);
  adv.loadAttribute("alpha_", alpha_);
}


} /* namespace OTROBOPT */
