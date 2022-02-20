import os
import subprocess

ban_list = [".git", ".vscode", ".vs", "obj", "bin"]

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
                ans.append(candidate)
    return ans



if not os.path.exists("obj"):
    os.mkdir("obj")

if not os.path.exists("bin"):
    os.mkdir("bin")


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

# spawn main target

print("creating main target...")
makefile_content += "kek.out: "
for cpp_file in srcs:
    filename_without_path = os.path.split(cpp_file)[-1]
    makefile_content += filename_without_path
    makefile_content += ".o "

makefile_content += "\n\tg++ -o bin/kek.out "


for cpp_file in srcs:
    filename_without_path = os.path.split(cpp_file)[-1]
    makefile_content += "obj/" + filename_without_path
    makefile_content += ".o "

makefile_content += "\n\n"

print("main target created")

# spawn targets for object files

print("creating make targets for .o ...")

cnt = 0
for cpp_file in srcs:
    filename_without_path = os.path.split(cpp_file)[-1]
    make_rule = subprocess.run(["g++", "-MM", "-MT", filename_without_path + ".o"] + cxx_include_argument + [cpp_file], stdout=subprocess.PIPE).stdout.decode("utf-8")
    makefile_content += make_rule
    makefile_content += "\t"
    filename_without_path = os.path.split(cpp_file)[-1]
    makefile_content += "g++ " + " ".join(cxx_include_argument) + " -c " + cpp_file + " -o obj/" + filename_without_path + ".o\n\n"
    cnt += 1
    print(cnt, "of", len(srcs))

print("targets created")
print("writing to file")

f = open("Makefile", "w")
f.write(makefile_content)
f.close()

print("completed")