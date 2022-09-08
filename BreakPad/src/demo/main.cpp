#include "client/linux/handler/exception_handler.h"

//定义一个回调函数来接收有关已写入的 minidump 的信息
static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
void* context, bool succeeded) {
  printf("Dump path: %s\n", descriptor.path());
  return succeeded;
}

void crash() { volatile int* a = (int*)(NULL); *a = 1; }

int main(int argc, char* argv[]) {
  google_breakpad::MinidumpDescriptor descriptor("/tmp");
  google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);
  crash();
  return 0;
}