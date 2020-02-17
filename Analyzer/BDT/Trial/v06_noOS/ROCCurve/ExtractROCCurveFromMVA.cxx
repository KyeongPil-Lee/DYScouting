#include <BDT/ROCCurve/ROCCurveProducer.h>

void ExtractROCCurveFromMVA()
{
  ROCCurveProducer* producer = new ROCCurveProducer("./");
  producer->Produce();
  producer->Save();
}