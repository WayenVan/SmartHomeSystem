/* 
 * This file is part of the SmartHomeSystem distribution (https://github.com/WayenVan/SmartHomeSystem).
 * Copyright (c) 2021 Jingyan Wang.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MODULE_EXCEPTION_HPP_
#define _MODULE_EXCEPTION_HPP_

#include <stdexcept>
#include <my_utils.hpp>

namespace wayenvan{

/**
 * @brief this is the self defined exception for
 * all modules, it greb current module's name with
 * error message
 */
class ModuleException: public std::runtime_error
{
    private:
    const char* kErrorClassName_;

    public:
    ModuleException(const char* kErrorClassName, const char* kMessage = ""):
        std::runtime_error(kMessage),
        kErrorClassName_(kErrorClassName)
        {}

    ~ModuleException() {}
    const char* get_error_module_name() const{
        return kErrorClassName_;
    } 
}; 

}
#endif