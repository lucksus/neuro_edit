#ifndef SPIKEEMITTER_H
#define SPIKEEMITTER_H
#include "spatialobject.h"
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>

class SpikeEmitter : public virtual SpatialObject
{
public:
    SpikeEmitter(Neuron*);

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SpatialObject>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar & boost::serialization::make_nvp("is_spiking", m_spiking);
    }

    bool is_spiking();

protected:
    SpikeEmitter(){}
    void emit_spike();
    void reset_spike_emitter();

private:
    bool m_spiking;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(SpikeEmitter);

namespace boost{
template<>
struct is_virtual_base_of<SpatialObject, SpikeEmitter>: public mpl::true_ {};
}
#endif // SPIKEEMITTER_H
