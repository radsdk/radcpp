#include <rad/System/Process.h>

namespace rad
{

std::vector<std::string> ExecuteAndReadLines(const std::string& command)
{
    using namespace boost;
    process::ipstream stream; // reading pipe-stream
    process::child child(command, process::std_out > stream);

    std::vector<std::string> lines;
    std::string line;
    while (child.running() && std::getline(stream, line) && !line.empty())
    {
        lines.push_back(line);
    }
    child.wait();
    return lines;
}

} // namespace rad
