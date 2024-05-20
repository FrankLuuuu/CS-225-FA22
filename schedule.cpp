/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

#include <cstring>

using namespace std;

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here! 
    V2D matrix;
    vector<string> row;
    vector<string> rows;
    vector<string> entries;
    string file = file_to_string(filename);
    SplitString(file, '\n', rows);

    for(string line : rows) {
        SplitString(line, ',', entries);
        for(string entry : entries) {
            entry = Trim(entry);
            row.push_back(entry);
        }
        matrix.push_back(row);
        entries.clear();
        row.clear();
    }
    
    return matrix;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & students){
    // YOUR CODE HERE
    V2D matrix;
    vector<string> row;
    string course;
    string student;
    bool studentExists = false;
    bool studentHasCourse = false;

    for(vector<string> roster : cv) {
        course = roster[0];
        row.push_back(course);

        for(size_t i = 1; i < roster.size(); i++) {
            student = roster[i];
            for(vector<string> listing : students) {
                if(listing[0] != student)
                    continue;
                studentExists = true;
                if(find(listing.begin(), listing.end(), course) == listing.end())
                    continue;
                studentHasCourse = true;
            }
            if(studentExists && studentHasCourse) {
                row.push_back(student);
                studentExists = false;
                studentHasCourse = false;
            }
        }

        if(row.size() > 1) 
            matrix.push_back(row);
            
        row.clear();
    }

    return matrix;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Your code here!
    int size = courses.size();
    vector<vector<int>> graph(size, vector<int>(size, 0));
    vector<string> course;
    string stu;

    for(int i = 0; i < size; i++) {
        course.push_back(courses[i][0]);
        for(int j = 0; j < size; j++) {
            if(i > j)
                graph[i][j] = graph[j][i];
            else if(i == j)
                graph[i][j] = 1;
            else 
                for(unsigned k = 1; k < courses[i].size(); k++) {
                    stu = courses[i][k];
                    if(find(courses[j].begin(), courses[j].end(), stu) != courses[j].end()) {
                        graph[i][j] = 1;
                        break;
                    }
                }
        }
    }
    
    V2D result;
    vector<string> row;
    vector<int> added;
    unsigned time = 0;
    bool valid = true;

    for(int i = 0; i < size; i++) {
        if(time == timeslots.size()) {
            valid = false;
            break;
        }

        if(find(added.begin(), added.end(), i) != added.end())
            continue;
        added.push_back(i);
        row.push_back(timeslots[time++]);
        row.push_back(course[i]);

        for(int j = i; j < size; j++) 
            if(!graph[i][j] && find(added.begin(), added.end(), j) == added.end()) {
                row.push_back(course[j]);
                added.push_back(j);
            }

        result.push_back(row);
        row.clear();
    }

    while(time != timeslots.size()) {
        row.push_back(timeslots[time++]);
        result.push_back(row);
        row.clear();
    }

    if(!valid) {
        result.clear();
        vector<string> temp;
        temp.push_back("-1");
        result.push_back(temp);
    }

    return result;
}