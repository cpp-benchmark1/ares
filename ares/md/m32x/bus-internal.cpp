auto M32X::readInternal(n1 upper, n1 lower, n32 address, n16 data) -> n16 {
  if(address >= 0x0000'0000 && address <= 0x0000'3fff) {
    if(shm.active()) shm.step(1); if(shs.active()) shs.step(1);
    if(shm.active()) return shm.bootROM[address >> 1];
    if(shs.active()) return shs.bootROM[address >> 1];
  }

  if(address >= 0x0000'4000 && address <= 0x0000'43ff) {
    return readInternalIO(upper, lower, address, data);
  }

  if(address >= 0x0200'0000 && address <= 0x023f'ffff) {
    while(dreq.vram) {
      // SH2 ROM accesses stall while RV is set
      if(shm.active()) shm.step(1);
      if(shs.active()) shs.step(1);
    }

    // TODO: SH2 ROM accesses need to stall while the m68k is on the bus
    if(shm.active()) shm.step(6); if(shs.active()) shs.step(6);
    return cartridge.child->read(upper, lower, address, data);
  }

  if(address >= 0x0400'0000 && address <= 0x0405'ffff) {
    if(shm.active()) shm.step(5); if(shs.active()) shs.step(5);
    return vdp.bbram[address >> 1 & 0xffff];
  }

  if(address >= 0x0600'0000 && address <= 0x0603'ffff) {
    return sdram[address >> 1 & 0x1ffff];
  }

  return data;
}

auto M32X::writeInternal(n1 upper, n1 lower, n32 address, n16 data) -> void {
  if(address >= 0x0000'4000 && address <= 0x0000'43ff) {
    return writeInternalIO(upper, lower, address, data);
  }

  if(address >= 0x0400'0000 && address <= 0x0401'ffff) {
    if (!vdp.framebufferAccess) return;
    if(!data && (!upper || !lower)) return;  //8-bit 0x00 writes do not go through
    if(shm.active()) shm.step(4); if(shs.active()) shs.step(4);
    if(upper) vdp.bbram[address >> 1 & 0xffff].byte(1) = data.byte(1);
    if(lower) vdp.bbram[address >> 1 & 0xffff].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x0402'0000 && address <= 0x0403'ffff) {
    if (!vdp.framebufferAccess) return;
    if(shm.active()) shm.step(4); if(shs.active()) shs.step(4);
    if(upper && data.byte(1)) vdp.bbram[address >> 1 & 0xffff].byte(1) = data.byte(1);
    if(lower && data.byte(0)) vdp.bbram[address >> 1 & 0xffff].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x0404'0000 && address <= 0x0405'ffff) {
    if (!vdp.framebufferAccess) return;
    if(!data && (!upper || !lower)) return;  //8-bit 0x00 writes do not go through
    if(shm.active()) shm.step(4); if(shs.active()) shs.step(4);
    if(upper) vdp.bbram[address >> 1 & 0xffff].byte(1) = data.byte(1);
    if(lower) vdp.bbram[address >> 1 & 0xffff].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x0600'0000 && address <= 0x0603'ffff) {
    if(upper) sdram[address >> 1 & 0x1ffff].byte(1) = data.byte(1);
    if(lower) sdram[address >> 1 & 0x1ffff].byte(0) = data.byte(0);
    return;
  }
}
