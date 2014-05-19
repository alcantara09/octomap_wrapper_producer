/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include "octomap_wrapper/OctomapWrapper.hpp"
#include "octomap/octomap.h"
#include <octomap/math/Utils.h>
#include "octomap_wrapper/conversion.hpp"

using namespace octomap_wrapper_producer;

Task::Task(std::string const& name, TaskCore::TaskState initial_state) :
		TaskBase(name, initial_state) {
	wrapper = new octomap_wrapper::OctomapWrapper();
	newmap = true;

}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine,
		TaskCore::TaskState initial_state) :
		TaskBase(name, engine, initial_state) {
	wrapper = new octomap_wrapper::OctomapWrapper();
	newmap = true;
}

Task::~Task() {
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook() {
	if (!TaskBase::configureHook())
		return false;

	return true;
}
bool Task::startHook() {
	if (!TaskBase::startHook())
		return false;
	return true;
}
void Task::updateHook() {

	if (newmap) {
		newmap = false;
		octomap::OcTree tree(0.05);  // create empty tree with resolution 0.1

		// insert some measurements of occupied cells
		octomap::point3d origin_cube(10.01f, 0.01f, 0.02f);
		for (int x = -20; x < 20; x++) {
			for (int y = -20; y < 20; y++) {
				for (int z = -20; z < 20; z++) {
					octomap::point3d endpoint((float) x * 0.05f,
							(float) y * 0.05f, (float) z * 0.05f);
					tree.updateNode(origin_cube + endpoint, true); // integrate 'occupied' measurement
				}
			}
		}

		// insert some measurements of free cells

		for (int x = -30; x < 30; x++) {
			for (int y = -30; y < 30; y++) {
				for (int z = -30; z < 30; z++) {
					octomap::point3d endpoint((float) x * 0.02f - 1.0f,
							(float) y * 0.02f - 1.0f, (float) z * 0.02f - 1.0f);
					tree.updateNode(origin_cube + endpoint, false); // integrate 'free' measurement
				}
			}
		}

		octomap::point3d origin(0.01f, 0.01f, 0.02f);
		octomap::point3d point_on_surface(2.01f, 0.01f, 0.01f);

		std::cout << "generating sphere at " << origin << " ..." << std::endl;

		for (int i = 0; i < 120; i++) {
			for (int j = 0; j < 120; j++) {
				if (!tree.insertRay(origin, origin + point_on_surface)) {
					std::cout << "ERROR while inserting ray from " << origin
							<< " to " << point_on_surface << std::endl;
				}
				point_on_surface.rotate_IP(0, 0, DEG2RAD(1.));
			}
			point_on_surface.rotate_IP(0, 0, DEG2RAD(-120.));
			point_on_surface.rotate_IP(0, DEG2RAD(1.), 0);
		}

		std::cout << "done." << std::endl;

		// -----------------------------------------------

		std::cout << "generating single rays..." << std::endl;

		int num_beams = 17;
		float beamLength = 3.0f;
		octomap::point3d single_origin(-6.0f, 0.45f, 0.45f);
		octomap::point3d single_endpoint(beamLength, 0.0f, 0.0f);

		for (int i = 0; i < num_beams; i++) {
			if (!tree.insertRay(single_origin,
					single_origin + single_endpoint)) {
				std::cout << "ERROR while inserting ray from " << single_origin
						<< " to " << single_endpoint << std::endl;
			}
			single_endpoint.rotate_IP(0, 0, DEG2RAD(360.0 / num_beams));
		}

		octomap_wrapper::binaryMapToMsg < octomap::OcTree > (tree, *wrapper);

	} else {
		octomap::OcTree tree =
				octomap::OcTree(
						"/home/alcantara/Documents/Rock/tutorials/orogen/octomap_wrapper_producer/tasks/fr_079.bt");
		octomap_wrapper::binaryMapToMsg < octomap::OcTree > (tree, *wrapper);
		newmap = true;
	}

	_output.write(*wrapper);
}
void Task::errorHook() {
	TaskBase::errorHook();
}
void Task::stopHook() {
	TaskBase::stopHook();
}
void Task::cleanupHook() {
	TaskBase::cleanupHook();
}
