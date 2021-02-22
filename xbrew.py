#!/usr/bin/env python3

import sys
import os
import json
import shutil

if __name__ == "__main__":
    argv = sys.argv
    argc = len(argv)

    templateNames = []
    with os.scandir("templates/") as it:
        for entry in it:
            if entry.is_dir() and not entry.name.startswith("_"):
                templateNames.append(entry.name)
    print()
    if argc < 3:
        print("Error: No template or project name provided")
        print("Usage:")
        print("xbrew <console_type> <project_name>")
        exit(1)

    consoleType = argv[1]
    projectName = argv[2]

    if not consoleType in templateNames:
        print("Sorry, this console is not supported yet.")
        exit(1)
    consoleType += "/"

    templatePath = "templates/" + consoleType
    projectPath = "./" + projectName + "/"
    if os.path.isdir(projectPath):
        print("Error: Project with the name '" + projectPath + "' already exists.")
        exit(1)
    
    shutil.copytree(templatePath, projectPath)
    
    def copyWithProjectName(name, loc, dest = ""):
        shutil.copyfile("templates/" + loc + name, projectPath + dest + name + "temp")
        with open(projectPath + dest + name + "temp", "r") as inf:
            with open(projectPath + dest + name, "w") as outf:
                for line in inf:
                    outf.write(line.replace("<PNAME>", projectName))
        os.remove(projectPath + dest + name + "temp")

    copyWithProjectName("Makefile", consoleType)
    copyWithProjectName("README.md", consoleType)
    copyWithProjectName(".gitignore", "_common/")

    # copyWithProjectName("run.sh", "_common/", "scripts/")
    # copyWithProjectName("build.sh", "_common/", "scripts/")
    # copyWithProjectName("debug.sh", "_common/", "scripts/")