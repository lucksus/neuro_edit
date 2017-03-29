// \file qstring_serialization.h
//  Enables serialization of Qt QString
//
//  Copyright (c) 2010 Florian Muecke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef QSTRING_SERIALIZATION_H
#define QSTRING_SERIALIZATION_H

#include <QString>
#include <string>
//#include <boost/archive/xml_iarchive.hpp>
//#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

namespace boost {
namespace serialization {


template<class Archive>
inline void save( Archive& ar, const QString& s, const unsigned int /*version*/ )
{
        using boost::serialization::make_nvp;
        std::string stdStr = s.toStdString();
        ar << make_nvp("value", stdStr);
}

template<class Archive>
inline void load( Archive& ar, QString& s, const unsigned int /*version*/ )
{
        using boost::serialization::make_nvp;

        std::string stdStr;
        ar >> make_nvp("value", stdStr);
        s = QString::fromStdString(stdStr);
}

template<class Archive>
inline void serialize( Archive& ar, QString& s, const unsigned int file_version )
{
        boost::serialization::split_free(ar, s, file_version);
}

} // namespace serialization
} // namespace boost

#endif // QSTRING_SERIALIZATION_H

