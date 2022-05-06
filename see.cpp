/**
 * @file see.cpp
 * @author Heache Bear (mrbeardad@qq.com)
 * @brief A command line tool to search markdown note and print it with
 * @version 3.1
 * @date 2022-04-07
 *
 * @copyright Copyright (c) 2020 - 2022
 *
 */
#include <sys/wait.h>
#include <unistd.h>

#include <array>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "md2ansi/md2ansi.hpp"
#include "utils/cmd.hpp"
#include "utils/sys.hpp"
#include "utils/tty.hpp"

auto main(int argc, char* argv[]) -> int {
    see::utils::CmdArgs cmd_args{};
    std::string text{};

    // If stdin is redirected to tty, ignore command arguments
    // and highlight the markdown text from stdin
    if (::isatty(STDIN_FILENO) != 1) {
        text.assign(std::istreambuf_iterator<char>{std::cin}, std::istreambuf_iterator<char>{});
    } else {
        cmd_args = see::utils::ParseCommandArgs(argc, argv);
        if (cmd_args.return_status != see::utils::kParseSuccess) {
            if (cmd_args.return_status == see::utils::kHelpOption) {
                return 0;
            }
            return 1;
        }
        text = see::utils::SearchEntries(cmd_args.files, cmd_args.regexes);
    }

    text = see::md2ansi::Highlight(text);

    using see::utils::HandleSyscall;

    if (!cmd_args.disable_pager && (HandleSyscall(::isatty(STDOUT_FILENO)) != 0)) {
        int output_row{1};
        int output_col{};
        size_t idx{};
        for (size_t newline_pos{};
             (newline_pos = text.find('\n', newline_pos)) != std::string::npos;) {
            ++output_row;
            output_col = std::max(output_col,
                                  see::utils::DisplayWidth(text.substr(idx, newline_pos - idx)));
            idx = ++newline_pos;
        }
        if (idx <= text.size()) {
            output_col = std::max(output_col, see::utils::DisplayWidth(text.substr(idx)));
        }
        if (auto win_size = see::utils::GetTtyWinSize();
            output_row > win_size.ws_row || output_col > win_size.ws_col) {
            std::array<int, 2> fds{};
            HandleSyscall(::pipe(fds.data()));

            if (auto pid = HandleSyscall(::fork()); pid == 0) {
                // Child process
                HandleSyscall(::dup2(fds[0], STDIN_FILENO));
                // Close copy of the pipe fd
                HandleSyscall(::close(fds[0]));
                HandleSyscall(::close(fds[1]));
                HandleSyscall(see::utils::ExecPager());
            } else {
                // Parent process
                HandleSyscall(::dup2(fds[1], STDOUT_FILENO));
                // close copy of the pipe fd
                HandleSyscall(::close(fds[0]));
                HandleSyscall(::close(fds[1]));
            }
        }
    }

    std::cout << text << std::endl;

    // Close the write end of pipe, trigger SIGPIPE
    HandleSyscall(::close(STDOUT_FILENO));
    // It is ok to emit the return value
    ::wait(nullptr);

    return 0;
}
