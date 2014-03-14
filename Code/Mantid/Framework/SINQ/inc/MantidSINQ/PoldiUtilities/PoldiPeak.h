#ifndef MANTID_SINQ_POLDIPEAK_H
#define MANTID_SINQ_POLDIPEAK_H

#include "MantidSINQ/DllConfig.h"
#include "MantidSINQ/PoldiUtilities/UncertainValue.h"
#include "MantidSINQ/PoldiUtilities/MillerIndices.h"

namespace Mantid {
namespace Poldi {

class MANTID_SINQ_DLL PoldiPeak
{
public:
    ~PoldiPeak() {}

    const MillerIndices& hkl() const;
    void setHKL(MillerIndices hkl);

    UncertainValue d() const;
    UncertainValue q() const;
    double twoTheta(double lambda) const;

    UncertainValue fwhm() const;
    UncertainValue intensity() const;

    void setD(UncertainValue d);
    void setQ(UncertainValue q);
    void setIntensity(UncertainValue intensity);
    void setFwhm(UncertainValue fwhm);

    static PoldiPeak create(UncertainValue qValue);
    static PoldiPeak create(UncertainValue qValue, UncertainValue intensity);

private:
    PoldiPeak(UncertainValue d = UncertainValue(), UncertainValue intensity = UncertainValue(), UncertainValue fwhm = UncertainValue(), MillerIndices hkl = MillerIndices());

    static UncertainValue dToQ(UncertainValue d);
    static UncertainValue qToD(UncertainValue q);

    MillerIndices m_hkl;

    UncertainValue m_d;
    UncertainValue m_q;
    UncertainValue m_intensity;
    UncertainValue m_fwhm;
};

}
}

#endif // MANTID_SINQ_POLDIPEAK_H
