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
   command_line_options.add_options()
         ("poc-period", boost::program_options::value<uint32_t>()->default_value(24), "Node Contribution Collection Period")
         ("poc-supervisor", boost::program_options::value<std::string>()->default_value("65.49.233.5:8093"), "Node Contribution Supervisor Address")
         ;
   config_file_options.add(command_line_options);
   return;
}

std::string poc_plugin::plugin_name()const
{
   return "poc";
}

void poc_plugin::plugin_initialize(const boost::program_options::variables_map& options)
{ try {
   ilog("poc plugin:  plugin_initialize() begin");
   if (options.count("poc-period")) {
       _collection_period = options["poc-period"].as<uint32_t>();
       ilog("###  period is ${p}", ("p", _collection_period));
   }
   if (options.count("poc-supervisor")) {
       _supervisor_addr = "ws://"+options["poc-supervisor"].as<std::string>();
       ilog("###  supervisor is ${a}", ("a", _supervisor_addr));
   }
   ilog("poc plugin:  plugin_initialize() end");
} FC_LOG_AND_RETHROW() }

void poc_plugin::plugin_startup()
{ try {
   ilog("poc plugin:  plugin_startup() begin");
   schedule_poc_loop();
   ilog("poc plugin:  plugin_startup() end");
} FC_CAPTURE_AND_RETHROW() }

void poc_plugin::plugin_shutdown()
{
   // nothing to do
}

void poc_plugin::schedule_poc_loop()
{
   fc::time_point now = fc::time_point::now();

   fc::time_point next_wakeup( now + fc::seconds( _collection_period ) );

   _contribution_collection_task = fc::schedule([this]{ _schedule_poc_loop(); },
                                   next_wakeup, "Collect Node Contribution");
}

void poc_plugin::_schedule_poc_loop()
{
   uint32_t contributions;
   ilog("_schedule_poc_loop()");

   // retrieve contributions
   contributions = disk_contribution();
   contributions += uptime_contribution();
   contributions += l2_contribution();

   // report contribution
   send_to();

   schedule_poc_loop();
}

uint32_t poc_plugin::disk_contribution()
{
   // Fix me
   return 1;
}

uint32_t poc_plugin::uptime_contribution()
{
   return _collection_period;
}

uint32_t poc_plugin::l2_contribution()
{
   return 1;
}

void poc_plugin::send_to()
{
    websocket_endpoint endpoint;
    int id;

    id = endpoint.connect(_supervisor_addr);
    fc::usleep(fc::seconds(2));
    endpoint.send(id, "{\"jsonrpc\": \"2.0\", \"id\": 0, \"method\": \"transfer\", \"params\": [\"nathan\", \"alpha\", 10, \"BTS\", \"good job\", true]}");
    fc::usleep(fc::seconds(2));
}
