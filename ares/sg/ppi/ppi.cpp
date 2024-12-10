#include <sg/sg.hpp>

namespace ares::SG1000 {

PPI ppi;
#include "serialization.cpp"

auto PPI::load(Node::Object parent) -> void {
  node = parent->append<Node::Object>("PPI");
}

auto PPI::readPortA() -> n8 {
  n8 output;

  if(Model::SG1000A()) {
    system.arcadeControls.poll();
    output.bit(0) = !system.arcadeControls.p1up->value();
    output.bit(1) = !system.arcadeControls.p1down->value();
    output.bit(2) = !system.arcadeControls.p1left->value();
    output.bit(3) = !system.arcadeControls.p1right->value();
    output.bit(4) = !system.arcadeControls.p1b1->value();
    output.bit(5) = !system.arcadeControls.p1b2->value();
    output.bit(6) = !system.arcadeControls.p1start->value();
    output.bit(7) = !system.arcadeControls.p2start->value();
    return output;
  }

  output.bit(0, 5) = controllerPort1.read();
  output.bit(6, 7) = controllerPort2.read().bit(0, 1);
  return output;
}

auto PPI::readPortB() -> n8 {
  n8 output;

  if(Model::SG1000A()) {
    system.arcadeControls.poll();
    output.bit(0) = !system.arcadeControls.p2up->value();
    output.bit(1) = !system.arcadeControls.p2down->value();
    output.bit(2) = !system.arcadeControls.p2left->value();
    output.bit(3) = !system.arcadeControls.p2right->value();
    output.bit(4) = !system.arcadeControls.p2b1->value();
    output.bit(5) = !system.arcadeControls.p2b2->value();
    output.bit(6) = !system.arcadeControls.service->value();
    output.bit(7) = !(system.arcadeControls.p1coin->value() || system.arcadeControls.p2coin->value());

    return output;
  }

  output.bit(0, 3) = controllerPort2.read().bit(2, 5);
  output.bit(4) = 1; // Set if cartridge pin b11 is not grounded
  output.bit(5) = 0; // Printer Fault (SC-3000)
  output.bit(6) = 0; // Printer Busy (SC-3000)
  output.bit(7) = 1; // Cassette Input (SC-3000)
  return output;
}

auto PPI::readPortC() -> n8 {
  debug(unimplemented, "PPI::readPortC()");
  return 0xff;
}

auto PPI::writePortC(n8 data) -> void {
  debug(unimplemented, "PPI::writePortC(0x", hex(data, 2), ")");
}

auto PPI::unload() -> void {
  node.reset();
}

}
