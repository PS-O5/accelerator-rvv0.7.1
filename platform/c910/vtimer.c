int get_vtimer()
{
  volatile unsigned int   LoadCount;
  asm ("csrr %[LoadCount], time\n"
      :[LoadCount]"=r"(LoadCount)
      :
      :
      );
  //LoadCount = *TIMER_ADDR;
  return LoadCount;
}

void sim_end()
{
  int *END_ADDR;
  END_ADDR = 0xA001FF48;
  unsigned int END_DATA;
  // END_DATA= 0xFFF;
  END_DATA= 0x444333222;
  *END_ADDR = END_DATA;
}
