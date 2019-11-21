/*
 * Copyright (c) 2018-2019 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "sf_common.hpp"
#include "sf_out.hpp"

namespace ams::sf {

    class IServiceObject{};

    class IMitmServiceObject : public IServiceObject {
        protected:
            std::shared_ptr<::Service> forward_service;
            os::ProcessId process_id;
            ncm::ProgramId  program_id;
        public:
            IMitmServiceObject(std::shared_ptr<::Service> &&s, os::ProcessId p, ncm::ProgramId r) : forward_service(std::move(s)), process_id(p), program_id(r) { /* ... */ }

            static bool ShouldMitm(os::ProcessId process_id, ncm::ProgramId program_id);
    };

    /* Utility. */
    #define SF_MITM_SERVICE_OBJECT_CTOR(cls) cls(std::shared_ptr<::Service> &&s, os::ProcessId p, ncm::ProgramId r) : ::ams::sf::IMitmServiceObject(std::forward<std::shared_ptr<::Service>>(s), p, r)

    template<typename T>
    struct ServiceObjectTraits {
        static_assert(std::is_base_of<ams::sf::IServiceObject, T>::value, "ServiceObjectTraits requires ServiceObject");

        static constexpr bool IsMitmServiceObject = std::is_base_of<IMitmServiceObject, T>::value;

        struct SharedPointerHelper {

            static constexpr void EmptyDelete(T *) { /* Empty deleter, for fake shared pointer. */ }

            static constexpr std::shared_ptr<T> GetEmptyDeleteSharedPointer(T *srv_obj) {
                return std::shared_ptr<T>(srv_obj, EmptyDelete);
            }

        };
    };



}