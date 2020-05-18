//--------------------------------------------------------------------------
// Copyright (C) 2020-2020 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------
// trace_config.cc author Oleksandr Serhiienko <oserhiie@cisco.com>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "trace_config.h"

#include <cstring>

#include "framework/module.h"
#include "managers/module_manager.h"

#include "trace_log_base.h"

TraceConfig::TraceConfig()
{
    auto modules = snort::ModuleManager::get_all_modules();
    for ( auto* module : modules )
    {
        if ( module->get_trace_options() )
            traces.emplace_back(*module);
    }
}

TraceConfig::~TraceConfig()
{
    delete logger_factory;
    logger_factory = nullptr;
}

bool TraceConfig::set_trace(const std::string& module_name, const std::string& trace_option_name,
    uint8_t trace_level)
{
    for ( auto& trace : traces )
    {
        if ( strcmp(trace.module_name(), module_name.c_str()) == 0 )
            return trace.set(trace_option_name, trace_level);
    }
    return false;
}

void TraceConfig::setup_module_trace() const
{
    for ( const auto& trace : traces )
        trace.set_module_trace();
}

void TraceConfig::enable_trace_snort()
{ trace_snort_enabled = true; }

void TraceConfig::merge_cmd_line(TraceConfig* trace_config)
{
    trace_snort_enabled = trace_config->trace_snort_enabled;
    
    if ( trace_snort_enabled )
        set_trace("snort", DEFAULT_TRACE_OPTION_NAME, DEFAULT_TRACE_LOG_LEVEL);
}
