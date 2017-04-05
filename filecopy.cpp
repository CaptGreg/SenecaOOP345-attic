// oop345 notes, section files.html
// https://scs.senecac.on.ca/~oop345/pages/content/files.html
// GB this code never compiled. There is a syntax error.
// GB: Insane code --> What if argc != 3 ??

// Copying Files
// fileCopy.cpp

 #include <fstream>

// int main(int argc, char agrv[]) {  // GB argv typo and missing '*'
int main(int argc, char *argv[]) {

    std::ifstream source     (argv[1]);
    std::ofstream destination(argv[2]);

    destination << source.rdbuf();
}
