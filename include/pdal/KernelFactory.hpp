/******************************************************************************
* Copyright (c) 2014, Bradley J Chambers (brad.chambers@gmail.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#pragma once

#include <pdal/pdal_internal.hpp>
#include <pdal/Kernel.hpp>
#include <pdal/KernelInfo.hpp>

#include <vector>
#include <map>
#include <string>


namespace pdal
{

// This class provides a mechanism for creating Kernel objects using only a
// string (the kernel type name) and an Options block.
//
// We keep a list of (kernel type name, creation function) pairs, which
// acts as a registry of creator functions.  The list is initialized with
// the core kernels we know about (I wish C++ had anonymous functions.).
// We allow the user to add his own "external" drivers to the registry list
// as well.
//
// We use 4 different functions for each kind of operation, since we have
// 4 types of derived classes from Kernel and they all have slightly different
// parameters.  That makes it kinda messy.

class PDAL_DLL KernelFactory
{
public:
    typedef Kernel* KernelCreator();

    typedef std::map<std::string, KernelCreator*> KernelCreatorList;

public:
    KernelFactory();

    std::unique_ptr<Kernel> createKernel(const std::string& type);

    void registerKernel(const std::string& type, KernelCreator* f);

    void loadPlugins();
    void registerPlugin(std::string const& filename);

    std::map<std::string, pdal::KernelInfo> const& getKernelInfos() const;
    template<class T> void registerKernelDriverInfo();

    // callers take ownership of returned kernels
    KernelCreator* getKernelCreator(const std::string& type) const;

private:
    void registerKnownKernels();

    // these are the "registries" of the factory creator functions
    KernelCreatorList m_kernelCreators;

    // driver name + driver description
    std::map<std::string, pdal::KernelInfo> m_driver_info;
    KernelFactory& operator=(const KernelFactory&); // not implemented
    KernelFactory(const KernelFactory&); // not implemented
};

template <class T>
inline void KernelFactory::registerKernelDriverInfo()
{

    pdal::KernelInfo info(T::s_getName(), T::s_getDescription());
    info.setInfoLink(T::s_getInfoLink());

    m_driver_info.insert(
        std::pair<std::string, pdal::KernelInfo>(T::s_getName(), info));
}

} // namespace pdal
