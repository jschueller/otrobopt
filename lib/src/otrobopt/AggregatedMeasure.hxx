//                                               -*- C++ -*-
/**
 *  @brief AggregatedMeasure
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
#ifndef OTROBOPT_AGGREGATEDMEASURE_HXX
#define OTROBOPT_AGGREGATEDMEASURE_HXX

#include "otrobopt/MeasureFunctionImplementation.hxx"
#include "otrobopt/MeasureFunction.hxx"

namespace OTROBOPT
{

/**
 * @class AggregatedMeasure
 *
 * Mean measure
 */
class OTROBOPT_API AggregatedMeasure
  : public MeasureFunctionImplementation
{
  CLASSNAME;

  typedef OT::Collection<MeasureFunction> MeasureFunctionCollection;
  typedef OT::PersistentCollection<MeasureFunction> MeasureFunctionPersistentCollection;

public:
  /** Default constructor */
  AggregatedMeasure();

  /** Parameter constructor */
  AggregatedMeasure(const MeasureFunctionCollection & collection);

  /** Virtual constructor method */
  AggregatedMeasure * clone() const;

  /** Evaluation */
  OT::NumericalPoint operator()(const OT::NumericalPoint & inP) const;

  /** Distribution accessor */
  virtual void setDistribution(const OT::Distribution & distribution);
  virtual OT::Distribution getDistribution() const;
  
  /** String converter */
  OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(OT::Advocate & adv);

private:
  MeasureFunctionPersistentCollection collection_;

}; /* class AggregatedMeasure */

} /* namespace OTROBOPT */

#endif /* OTROBOPT_AGGREGATEDMEASURE_HXX */
