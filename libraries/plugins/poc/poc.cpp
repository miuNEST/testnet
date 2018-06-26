/*
 * Copyright (c) 2018 uNest, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <graphene/poc/poc.hpp>

#include <graphene/chain/database.hpp>
#include <graphene/chain/witness_object.hpp>

#include <graphene/utilities/key_conversion.hpp>

#include <fc/smart_ref_impl.hpp>
#include <fc/thread/thread.hpp>

#include <iostream>

using namespace graphene::poc_plugin;
using std::string;
using std::vector;

namespace bpo = boost::program_options;

void poc_plugin::plugin_set_program_options(
   boost::program_options::options_description& command_line_options,
   boost::program_options::options_description& config_file_options)
{
   return;
}

std::string poc_plugin::plugin_name()const
{
   return "poc";
}

void poc_plugin::plugin_initialize(const boost::program_options::variables_map& options)
{ try {
   ilog("poc plugin:  plugin_initialize() begin");
   ilog("poc plugin:  plugin_initialize() end");
} FC_LOG_AND_RETHROW() }

void poc_plugin::plugin_startup()
{ try {
   ilog("poc plugin:  plugin_startup() begin");
   ilog("poc plugin:  plugin_startup() end");
} FC_CAPTURE_AND_RETHROW() }

void poc_plugin::plugin_shutdown()
{
   // nothing to do
}

