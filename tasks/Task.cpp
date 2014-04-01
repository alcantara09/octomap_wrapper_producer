/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include "octomap_wrapper/OctomapWrapper.hpp"
#include "octomap/octomap.h"
#include "octomap_wrapper/conversion.hpp"

using namespace octomap_wrapper_producer;

Task::Task(std::string const& name, TaskCore::TaskState initial_state) :
		TaskBase(name,initial_state) {
	wrapper = new octomap_wrapper::OctomapWrapper();

}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine,TaskCore::TaskState initial_state) :
		TaskBase(name, engine, initial_state) {
	wrapper = new octomap_wrapper::OctomapWrapper();
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
	TaskBase::updateHook();

	octomap::OcTree tree_bt(0.1);

	for (int x = -20; x < 20; x++) {
		for (int y = -20; y < 20; y++) {
			for (int z = -20; z < 20; z++) {
				octomap::point3d endpoint((float) x * 0.05f, (float) y * 0.05f,
						(float) z * 0.05f);
				tree_bt.updateNode(endpoint, true); // integrate 'occupied' measurement
			}
		}
	}

	// insert some measurements of free cells

	for (int x = -30; x < 30; x++) {
		for (int y = -30; y < 30; y++) {
			for (int z = -30; z < 30; z++) {
				octomap::point3d endpoint((float) x * 0.02f - 1.0f,
						(float) y * 0.02f - 1.0f, (float) z * 0.02f - 1.0f);
				tree_bt.updateNode(endpoint, false); // integrate 'free' measurement
			}
		}
	}

	octomap_wrapper::binaryMapToMsg < octomap::OcTree > (tree_bt, *wrapper);

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
