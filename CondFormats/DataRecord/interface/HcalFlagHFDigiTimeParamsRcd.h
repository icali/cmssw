#ifndef HcalFlagHFDigiTimeParamsRcd_H
#define HcalFlagHFDigiTimeParamsRcd_H
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/HcalRecNumberingRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
class HcalFlagHFDigiTimeParamsRcd : public edm::eventsetup::DependentRecordImplementation<
                                        HcalFlagHFDigiTimeParamsRcd,
                                        edm::mpl::Vector<HcalRecNumberingRecord, IdealGeometryRecord> > {};
#endif
