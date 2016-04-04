// SWIG file

%{
#include "otrobopt/MeasureFunctionImplementation.hxx"
%}

%include MeasureFunctionImplementation_doc.i

%include otrobopt/MeasureFunctionImplementation.hxx
namespace OTROBOPT { %extend MeasureFunctionImplementation { MeasureFunctionImplementation(const MeasureFunctionImplementation & other) { return new OTROBOPT::MeasureFunctionImplementation(other); } } }
