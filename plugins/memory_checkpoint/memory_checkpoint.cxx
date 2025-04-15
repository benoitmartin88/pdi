/*******************************************************************************
 * Copyright (C) 2025 Commissariat a l'energie atomique et aux energies alternatives (CEA)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of CEA nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include <iostream>
#include <string>


#include <dlfcn.h>

#include <pdi.h>
#include <pdi/context.h>
#include <pdi/data_descriptor.h>
#include <pdi/expression.h>
#include <pdi/logger.h>
#include <pdi/paraconf_wrapper.h>
#include <pdi/plugin.h>
#include <pdi/ref_any.h>

namespace {

using namespace PDI;


struct memory_checkpoint_plugin : public Plugin {
	memory_checkpoint_plugin(Context& ctx, PC_tree_t conf)
		: Plugin{ctx}
	{
  // Loading configuration for events
    // PC_tree_t on_event = PC_get(conf, ".on_event");
    // if (!PC_status(on_event))
    //   each(on_event, [&](PC_tree_t event_name, PC_tree_t events) {
    //     opt_each(events, [&](PC_tree_t one_event) {
    //       each(one_event, [&](PC_tree_t function_name, PC_tree_t parameters) {
    //       });
    //     });
    //   });

    // // Loading configuration for data
    // PC_tree_t on_data = PC_get(conf, ".on_data");
    // if (!PC_status(on_data))
    //   each(on_data, [&](PC_tree_t data_name, PC_tree_t datas) {
    //     opt_each(datas, [&](PC_tree_t one_data) {
    //       each(one_data, [&](PC_tree_t function_name, PC_tree_t parameters) {
    //       });
    //     });
    //   });
    
		ctx.logger().info("Plugin loaded successfully");
	}

	~memory_checkpoint_plugin() { context().logger().info("Closing plugin"); }

	/** Pretty name for the plugin that will be shown in the logger
	 *
	 * \return pretty name of the plugin
	 */
	static std::string pretty_name() { return "Memory-checkpoint-plugin"; }

}; // struct memory_checkpoint_plugin

} // namespace

PDI_PLUGIN(memory_checkpoint)
