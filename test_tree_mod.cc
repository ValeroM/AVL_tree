/*
Main file for part 2c of the Project #2
Author: Marco Valero
File Title: test_tree_mod.cc

Description: Same functionalitues as part2b of the project. However, double rotations for the tree are implemented directly instead of calling the two single rotations. The modified versions of the rotations are under the file: avl_tree_modified.h

Date of creation: February 25nd, 2019
Date of completition: March 9th, 2019

Purpose: To get comfortable with the properties of AVL trees and its implementations
*/

#include "avl_tree_modified.h"
#include "SequenceMap.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <string>

// Return false if we have reached two slashes back to back
bool GetNextRecognitionSequence(const std::string &line_of_file, std::string &a_reco_seq, std::size_t &first_slash_pos) {
  // finds the position of the next slash after the first slash
  std::size_t pos_of_next_slash = line_of_file.find("/", first_slash_pos+1);
  // this length is needed since substr needs the number of characters to include in a new string
  int length_of_seq_to_parse = pos_of_next_slash - first_slash_pos;
  //  return false if there are two slashes next to each other
  if (pos_of_next_slash - first_slash_pos == 1) {
    return false;
  } else {
      // using the position of the slahses, we make the recognition sequence equal to the sequence in the line of the file
      a_reco_seq = line_of_file.substr(first_slash_pos+1, length_of_seq_to_parse-1);
      // update slash for next call
      first_slash_pos = pos_of_next_slash;
      return true;
    }
}

// gets the enzyme (i.e., the string before the first slash)
std::string GetEnzymeAcronym(const std::string &line_of_file, const std::size_t &first_slash_pos) {
  return (line_of_file.substr(0, first_slash_pos)); // return the enzyme
}

// checks is the line from the file "rebase210.txt" is valid
bool GoodLine(const std::string &line_of_file) {
  return (isalnum(line_of_file[0]) || line_of_file[0] == '\'');
}

// Prints out the new data after removing the nodes that matches with every other sequence from the sequence.txt file
template <typename TreeType>
void RedoStepsAfterRemovingNodes(const TreeType &a_tree) {
  std::cout << "6a: " << a_tree.NumOfNodes() << std::endl;
  std::cout << "6b: " << a_tree.AverageDepthOfTree() << std::endl;
  std::cout << "6c: " << a_tree.RatioOfAverageDepth() << std::endl;
}

/*
Removes every other sequence in sequences.txt from the tree and counts the
total number of recursion calls for all executions of RemoveEveryOther().
*/
template <typename TreeType>
void RemoveEveryOtherLineTest(const std::string &seq_filename, TreeType &a_tree) {
  std::ifstream file_two;
  file_two.open(seq_filename);
  if (file_two.fail()) {
    std::cerr << "Error -- Cannot read from file. Exit\n";
    exit(1);
  }
  std::string line_of_sec_file, every_other_sequence;
  int calls = 0, succesfull_removes = 0, number_of_remove_queries = 0, total_calls = 0;
  float avg_num_of_recur_calls = 0;
  while(std::getline(file_two, every_other_sequence)) {
    number_of_remove_queries++; // total number of "every other" lines in total
    if (GoodLine(every_other_sequence)) {
      if (a_tree.RemoveEveryOther(every_other_sequence, calls))
        succesfull_removes++;
      total_calls += calls;
      calls = 0; // reset because RemoveEveryOther() modifies the number with every call
    }
    std::getline(file_two, line_of_sec_file); // read next line in order to skip it
  }
  avg_num_of_recur_calls = (static_cast<float>(total_calls)/static_cast<float>(number_of_remove_queries));
  std::cout << "5a: " << succesfull_removes << std::endl;
  std::cout << "5b: " << avg_num_of_recur_calls << std::endl;
  RedoStepsAfterRemovingNodes(a_tree);
}

/*
Searches the tree for each string in the sequences.txt file and counts the total number of recursive calls for all executions of FindEachSequence().
*/
template <typename TreeType>
void TestTreeSecondFile(const std::string &seq_filename, const TreeType &a_tree) {
  std::ifstream file_two;
  file_two.open(seq_filename);
  if (file_two.fail()) {
    std::cerr << "Error -- Cannot read from file. Exit\n";
    exit(1);
  }
  std::string lines_of_sec_file;
  int querries_found = 0, total_calls = 0, calls = 0, number_of_queries = 0;
  float avg_num_of_recur_calls = 0;
  while(std::getline(file_two, lines_of_sec_file)) {
    if (GoodLine(lines_of_sec_file)) {
      number_of_queries++; // number of querries made
      if (a_tree.find(lines_of_sec_file, calls))
        querries_found++;
      total_calls += calls;
      calls = 0; // reset because find_each_sequence() modifies the number with every call
    }
  }
  avg_num_of_recur_calls = (static_cast<float>(total_calls)/static_cast<float>(number_of_queries));
  std::cout << "4a: " << querries_found << std::endl;
  std::cout << "4b: " << avg_num_of_recur_calls << std::endl;
}


// @db_filename: an input database filename.
// @seq_filename: an input sequences filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be
// empty.
template <typename TreeType>
void TestTree(const std::string &db_filename, const std::string &seq_filename, TreeType &a_tree) {
  std::ifstream file;
  file.open(db_filename);
  if (file.fail()) {
    cerr << "Error -- Cannot read from file. Exit\n";
    exit(1);
  }
  std::string lines_of_text, header_to_ignore, an_enz_acro;
  for (int i = 0; i < 10; i++) {
    getline(file, header_to_ignore);
  }
  while(getline(file, lines_of_text)) {
    if (isalnum(lines_of_text[0])) {
      std::size_t first_slash_pos = lines_of_text.find('/');
      an_enz_acro = GetEnzymeAcronym(lines_of_text, first_slash_pos);
      std::string a_reco_seq;
      while (GetNextRecognitionSequence(lines_of_text, a_reco_seq, first_slash_pos)) {
        // filling tree with nodes of type SequenceMap
        SequenceMap new_sequence_map(a_reco_seq, an_enz_acro);
        a_tree.insert(new_sequence_map);
      }
    }
  }
  std::cout << "2: " << a_tree.NumOfNodes() << std::endl;
  std::cout << "3a: " << a_tree.AverageDepthOfTree() << std::endl;
  std::cout << "3b: " << a_tree.RatioOfAverageDepth() << std::endl;
}


int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <databasefilename> <queryfilename>" << std::endl;
    return 0;
  }
  const std::string db_filename(argv[1]);
  const std::string seq_filename(argv[2]);
  std::cout << "Input file is " << db_filename << " and sequences file is " << seq_filename << std::endl;
  std::cout << "Type of tree is AVL " << std::endl;
  AvlTree<SequenceMap> a_tree;
  // Fills the tree and outputs number of total nodes, average depth and ratio of average depth
  TestTree(db_filename, seq_filename, a_tree);
  // Searches the tree for each string in the sequences.txt file and counts the total number of recursive calls for all executions of FindEachSequence().
  TestTreeSecondFile(seq_filename, a_tree);
  // Removes every other sequence in sequences.txt from the tree and counts the total number of recursion calls for all executions of RemoveEveryOther().
  RemoveEveryOtherLineTest(seq_filename, a_tree);
  return 0;
}
