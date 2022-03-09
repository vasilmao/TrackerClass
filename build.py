#!/usr/bin/env python3

import os
import subprocess

BIN_DIR = "bin"
OBJ_DIR = "obj"


ban_list = [".git", ".vscode", ".vs", OBJ_DIR, BIN_DIR, "targets"]
LXX_FLAGS = []
CXX_FLAGS = [
    "-std=c++17",
    # "-fno-elide-constructors",
    "-DMISTAKE"
    ]

CXX = "g++"


def get_dirs_rec(start_dir):
    ans = [start_dir]
    for subdir in os.listdir(start_dir):
        if (any(subdir == x for x in ban_list)):
            continue
        candidate = os.path.join(start_dir, subdir)
        if (os.path.isdir(candidate)):
            ans += get_dirs_rec(candidate)
    return ans

def get_src_rec(start_dir):
    ans = []
    for subdir in os.listdir(start_dir):
        if (any(subdir == x for x in ban_list)):
            continue
        candidate = os.path.join(start_dir, subdir)
        if (os.path.isdir(candidate)):
            ans += get_src_rec(candidate)
        elif (os.path.isfile(candidate)):
            if candidate.endswith(".cpp"):
                ans.append(candidate[:candidate.rfind(".")])
    return ans



if not os.path.exists(OBJ_DIR):
    os.mkdir(OBJ_DIR)

if not os.path.exists(BIN_DIR):
    os.mkdir(BIN_DIR)


print("getting directories...")
dirs = get_dirs_rec(os.path.curdir)
print("directories got")
print("creating g++ -I arguments")

# cxx_include_argument = " ".join("-I " + x for x in dirs)
cxx_include_argument = []
for dir in dirs:
    cxx_include_argument.append("-I")
    cxx_include_argument.append(dir)

print("arguments created")

# now lets find all .cpp files

print("finding .cpp files...")
srcs = get_src_rec(os.path.curdir)
print("list of .cpp files created")

# print(dirs)
# print(srcs)
# print(cxx_include_argument)

makefile_content = ""


# spawn main targets

targets_list = []

makefile_content += "all: "

for file in os.listdir(os.path.join(os.path.curdir, "targets")):
    print("target file:", file)
    file_name_without_extension = file[:file.rfind(".")]
    target_file = os.path.join(os.path.curdir, "targets", file_name_without_extension)
    if (os.path.isfile(target_file + ".cpp")):
        out_filename = BIN_DIR + "/" + file_name_without_extension + ".out"
        makefile_content += out_filename + " "

makefile_content += "\n\n"

for file in os.listdir(os.path.join(os.path.curdir, "targets")):
    print("target file:", file)
    file_name_without_extension = file[:file.rfind(".")]
    target_file = os.path.join(os.path.curdir, "targets", file_name_without_extension)
    if (os.path.isfile(target_file + ".cpp")):
        targets_list.append(target_file)

        out_filename = BIN_DIR + "/" + file_name_without_extension + ".out"
        makefile_content += out_filename + ": Makefile "
        makefile_content += OBJ_DIR + "/" + file_name_without_extension + ".o "
        for cpp_file in srcs:
            filename_without_path = os.path.split(cpp_file)[-1]
            makefile_content += OBJ_DIR + "/" + filename_without_path
            makefile_content += ".o "

        # makefile_content += "\n\t" + CXX + " " + " ".join(LXX_FLAGS) + " -o "  + out_filename + " "
        makefile_content += "\n\t" + CXX + (" " + " ".join(LXX_FLAGS) if len(LXX_FLAGS) != 0 else "")+ " -o " + out_filename + " "

        makefile_content += OBJ_DIR + "/" + file_name_without_extension + ".o "

        for cpp_file in srcs:
            filename_without_path = os.path.split(cpp_file)[-1]
            makefile_content += OBJ_DIR + "/" + filename_without_path
            makefile_content += ".o "
        print("target spawned", file)
        makefile_content += "\n\n"

print("main target created")

srcs += targets_list

# spawn targets for object files

print("creating make targets for .o ...")

cnt = 0
for cpp_file in srcs:
    filename_without_path = os.path.join(OBJ_DIR, os.path.split(cpp_file)[-1])
    # filename_without_path = cpp_file
    make_rule = subprocess.run([CXX, "-MM", "-MT", filename_without_path + ".o"] + cxx_include_argument + [cpp_file + ".cpp"], stdout=subprocess.PIPE).stdout.decode("utf-8")
    make_rule = make_rule.rstrip() + " Makefile"
    makefile_content += make_rule
    makefile_content += "\n\t"
    filename_without_path = os.path.split(cpp_file)[-1]
    makefile_content += CXX + " " + " ".join(cxx_include_argument) + " " + " ".join(CXX_FLAGS) + " -c " + cpp_file + ".cpp -o " + OBJ_DIR + "/" + filename_without_path + ".o\n\n"
    cnt += 1
    print(cnt, "of", len(srcs))

print("targets created")
print("writing to file")

f = open("Makefile", "w")
f.write(makefile_content)
f.close()

print("completed")