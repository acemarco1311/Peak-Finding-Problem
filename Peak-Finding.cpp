#include <iostream>
#include <map> 
#include <iomanip>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

int find_one_peak(int* array, int length); //bruteforce approach finding a peak in one dimension array
int find_one_peak_binary_search(int* array, int length); //divide conquer approach finding a peak in one dimension array
int greedy_ascent(int** array, int n, int m); //bruteforce approach finding a peak in 2 dimension array
int finding_one_peak_binary_search_two_dimension(int** array, int n, int m); //divide conquer approach finding a peak in 2d array

/*
 * given array = [a,b,c]
 * element b is a peak if and only if b>=a and b>=c 
 * element c is a peak if and only if c>=b
 * element a is a peak if and only if a>=b 
 * any array will have at least 1 peak, the maximum element of the given array is also a peak
*/ 

//find one peak of the given array - we could simply find the highest element of the array
//brute-force approach
int find_one_peak(int* array, int length){
    int result = *array;
    for(int i=0; i<length; i++){
        if(*(array+i) >= result){
            result = *(array+i);
        }
    }
    return result;
}

//find all the peaks of the given array, store them in a vector, return the pointer to that array
//brute-force approach
vector<int>* find_all_peaks(int* array, int length){
    vector<int>* pointer = new vector<int>(1);
    if((*array) >= *(array+1)){
        pointer->push_back(*array);
    }
    if((*array+length-1) >= (*array+length-2)){
        pointer->push_back(*array+length-1);
    } 
    for(int i=1; i<length-1; i++){
        if(*(array + i) >= *(array + i + 1) && *(array + i) >= *(array + i - 1)){
            pointer->push_back(*(array + i));
        }
    }
    return pointer; 
}

//find the only peak of the given array, we just break the array to smaller array
//starting from the middle and delete a half of array, continue with another half 
//we just guess which half is likely to have peak near the middle, therefore we could get the peak quickly
//it is no matter which side we continue because whichever we will eventually get one peak (becase any array has at least 1 peak)
//but we care which side will have a peak nearest to the middle therefore we could get that quicker
//divide conquer approach (binary search)
//for example, given the array [a, b, c, d, e]
//             start from middle of array (element c) 
//             if(c>=b && c>=d) -> return c because c is a peak
//             else if(b>d) -> delete from d to the end, call the function again (recursive)
//             else if(b<d) -> delete from the start to b, call the function again (recursive)
//             else if(b=d) -> delete random side, call function, 
//                  --> in this case, we cannot guess which side is likely to have a peak nearest, we just random
//                      we eventually get another peak, because every array has peak

int find_one_peak_binary_search(int* array, int length){  //using binary search o(log2(n))
    if(length == 1){
        return array[0]; //base case to stop recursion, when the array has 1 element (cannot cut in half anymore), return that element.
    }
    else if(array[0] >= array[1]){ //check the first element
        return array[0];   //if the first is peak, return the first element and stop
    }
    else if(array[length-1] >= array[length-2]){ //check the last element   
        return array[length-1]; //if the last element is peak, return the last element
    }
    int middle = length/2; //get the middle element of the array
    if(array[middle] >= array[middle +1] && array[middle] >= array[middle-1]){ //compare middle element to 2 elements around it
       return array[middle];  //if the middle both larger than 2 others around, it is a peak   
    }
    else if(array[middle-1] > array[middle+1]){ //if the middle is not peak, and the element before larger than the element after
       int new_length = middle;
       int* pointer = new int[new_length];
       for(int i=0; i<new_length; i++){   //cut the second half (from the middle to the end) 
           pointer[i] = array[i];
       }
       return find_one_peak_binary_search(pointer, new_length); //recursion to check the first half
    }
    else{
       int new_length = length - middle - 1; //if the element after larger than element before
       int* pointer = new int[new_length];
       int index = 0;
       for(int i=middle+1; i<length; i++){ //cut the first half (from 0 to middle) 
           pointer[index] = array[i];
           index++;
       }
       return find_one_peak_binary_search(pointer, new_length); //check the second half 
    }
    return 0;
}


int binary_peak_finding(int* array, int start, int end){ //this is the improved version
    int middle = start + ((end-start)/2); //middle of the array 
    if(start >= end){ //base case for recursion
        return array[start]; //if the input array has only 1 element
    }
    else if(array[middle] >= array[middle-1] && array[middle] >= array[middle+1]){
        return array[middle];
    }
    else if(array[middle-1] >= array[middle+1]){
        //the left element > right
        //get the left half
        return binary_peak_finding(array, start, middle-1);
    }
    else if(array[middle+1] > array[middle-1]){
        //right element > left element
        //get the right half
        return binary_peak_finding(array, middle+1, end);
    }
    else{
        return 0;
    }
}

//GREEDY ASCENT ALGORITHM, finding peak in 2D array, bruteforce approach, O(n*m) 
/* PSEUDOCODE
1. Pickup a random element in the 2D array (usually the middle of the array)
2. Then compare that element to other elements around it (neigbours) (Make sure when the current element is on the edge, make sure you will handle OutOfBound error (compare with those outside the array))
    2.1 If there is no neighbour that larger or equal than the current element
        Return current element
    2.2 Else
        2.2.1 Pick the largest neigbour to be the current element
        2.2.2 Go back to step 1
*/

string get_largest_neigbour(std::map<string, int> neigbour){
    int* values = new int[9];
    std::map<string, int>::iterator itr;
    int index = 0;
    for(itr = neigbour.begin(); itr != neigbour.end(); ++itr){
        *(values + index) = itr->second;
        index++;
    }
    int largest = *values;
    for(int i = 0; i<9; i++){
        if(*(values + i) > largest){
            largest = *(values + i);
        }
    }
    string result = "";
    for(itr = neigbour.begin(); itr != neigbour.end(); ++itr){
        if(itr->second == largest){
            result = itr->first;
        }
    }
    return result;
}

int greedy_ascent(int** array, int n, int m){
    int n_middle = n/2; //get middle element of the array
    int m_middle = m/2; 
    bool complete = false; //assign true when get the peak and stop while loop  
    int current_row_index = n_middle; //store the current element that is operating on, first current element is middle
    int current_column_index = m_middle; //because we start at middle
    bool is_peak = true;
    string result = ""; //store the default value of result to a string because there is no valid default value of result
                        //using string then we could know that if we did get a peak or not 
    std::map<string, int> neigbour;  // store the neigbour value as index = "12" [1][2], and value as int 
    while(complete == false){ 
        for(int index_row = current_row_index - 1; index_row < current_row_index+2; index_row++){
            if(index_row < 0 || index_row >= n){ //handle OutOfBound error with elements on the edge
                continue;
            }
            for(int index_column = current_column_index - 1; index_column < current_column_index+2; index_column++){
                if(index_column < 0 || index_column >= m){ //handle OutOfBound error with elements on the edge
                    continue;
                }
                else if(array[current_row_index][current_column_index] < array[index_row][index_column]){ //the current element is not a peak
                    is_peak = false;
                }
                string temp = ""; //store the index of current element
                temp += to_string(index_row);
                temp += " "; //add a space between 2 indexes
                temp += to_string(index_column);
                neigbour.insert(pair<string ,int>(temp, array[index_row][index_column]));
            }
        }
        if(is_peak == true){
            //is_peak variable is still true, the current element is the peak, return it 
            complete = true; //stop the loop
            result = to_string(array[current_row_index][current_column_index]); //cast the peak to a string

        }
        else{
            //is_peak is false meaning that the current element is not a peak
            is_peak = true; //setting is_peak for the next operation
            string new_current_index = get_largest_neigbour(neigbour); //get the index of largest neigbour 
            //current_row_index = (int)new_current_index[0] - 48; //cast a char to int need - 48 (ASCII) 
            current_row_index = std::stoi(new_current_index.substr(0, new_current_index.find(" ")-0+1)); //str.substr(start, end-start+1), get from the start to the first space 
            current_column_index = std::stoi(new_current_index.substr(new_current_index.find(" ")+1, new_current_index.length()-new_current_index.find(" ")+1+1)); //str.subst(start, end-start+1), get from the first space + 1 to the end of the string
            //current_column_index = (int)new_current_index[1] - 48; //set the next one to the largest neighbour 
            neigbour.clear(); //clear neigbour of current element to store next element's neigbours

        }
    }
    if(result != ""){
        return std::stoi(result); //cast a string to int 
    }
    else{
        cout<<"Cannot find any peak of the given array"<<endl;
        return -1;
    }
} 

int greedy_ascent_improved(int** array, int row, int column){
    int current_row = row/2; //start from the middle element 
    int current_column = column/2;
    int max_neighbour_row = current_row; //store the largest neighbour of current element
    int max_neighbour_column = current_column;
    bool complete = false; //repeat until the current element is the peak
    while (complete == false){
        for(int i=current_row-1; i<= current_row+1; i++){ //check the neigbour
            if(i<0 || i>= row){ //handle out of bound 
                continue;
            }
            for(int j=current_column-1; j<= current_column+1; j++){ //check neighbour
                if(j<0 || j>=column){ //handle out of bound
                    continue;
                }
                else if(array[i][j] > array[max_neighbour_row][max_neighbour_column]){ //update the index of max neigbour
                    max_neighbour_row = i;
                    max_neighbour_column = j;
                }
            }
        }
        if(current_row == max_neighbour_row && current_column == max_neighbour_column){ //if current element = max neighbour then current is the peak
            complete = true; //stop the while loop
        }
        else if(current_row != max_neighbour_row || current_column != max_neighbour_column){ //set the max neighbour to be the next current element
            current_row = max_neighbour_row; //continue the loop
            current_column = max_neighbour_column;
        }
    }
    return array[current_row][current_column]; //return
}

//Recursive method to find a peak in 2D array with time complexity O(nlog(n))
/* 
 
    ---->Base case: if the 2D array has only 1 column, return the maximum element of that column
    1. Pick the middle column of 2D array: j=m/2 
    2. Find the maximum element of column j: the element will be a[i][j]
    3. Compare left and right of the maximum element of the column: a[i][j-1] vs a[i][j] vs a[i][j+1]
        If a[i][j] >= a[i][j-1], a[i][j+1]
            Return the element a[i][j] because it is the peak
        Else if a[i][j] is not larger than both a[i][j-1] and a[i][j+1]
            If a[i][j-1] > a[i][j+1] (left larger than right)
                Get the left columns, delete the right columns (get from col 0 to col j)
                Call the function again on the left columns
            Else a[i][j+1] > a[i][j-1] (right larger than left)
                Get the right columns, delete left columns (get from col j to end of column) 
                Call the function again
 */ 

int get_max_row_index(int** array, int n, int m, int column){ //finding the index of max element in 1d array
    int max = array[0][column];
    int max_row_index = 0;
    for(int i=0; i<n; i++){
        if(array[i][column] > max){
            max = array[i][column];
            max_row_index = i;
        }
    }
    return max_row_index;
}

int find_one_peak_binary_search_two_dimension(int** array, int row, int column, int middle){

    int max_row_index = get_max_row_index(array, row, column, middle); //get index of global max value in the middle column
    if(middle == 0 && array[max_row_index][middle] >= array[max_row_index][middle+1]){ //base case middle = first column 
        return array[max_row_index][middle]; //return the global max
    }
    else if(middle == column-1 && array[max_row_index][middle] >= array[max_row_index-1][middle]){ //base case middle = last column
        return array[max_row_index][middle]; //return global max
    }
    else if(array[max_row_index][middle] >= array[max_row_index][middle+1] && array[max_row_index][middle-1]){ //best case, global max is the peak
        return array[max_row_index][middle]; //return the global max 
    }
    else if(array[max_row_index][middle-1] >= array[max_row_index][middle+1]){ //global max is not peak and left > right 
        return find_one_peak_binary_search_two_dimension(array, row, column, middle - ceil((double)middle/2)); //get the left half
    }
    else if(array[max_row_index][middle+1] > array[max_row_index][middle-1]){ //global max is not peak and right > left
        return find_one_peak_binary_search_two_dimension(array, row, column, middle + ceil((double)middle/2)); //get the right half
    }
    else{
        return 0;
    }
} 



int main(){
    int array[10] = {1,2,4,5,6,2,5,7,8,4};
    cout<<"One peak of the array is: " << find_one_peak(array, 10) << endl;
    cout<<"One peak of the array using binary search is: " << find_one_peak_binary_search(array,10) << endl;    
    int** double_pointer = new int*[4];
    int first_array[3] ={1,2,3};
    int second_array[3]={4,5,6};
    int third_array[3]= {7,8,0};
    int fourth_array[3]={1,4,0};
    double_pointer[0] = first_array;
    double_pointer[1] = second_array;
    double_pointer[2] = third_array;
    double_pointer[3] = fourth_array;
    cout<< "Peak of 2D array using Greead Ascent algorithm: " << greedy_ascent(double_pointer, 4, 3)<<endl;
    cout<< "Peak of 2D array using binary search algorithm: " << find_one_peak_binary_search_two_dimension(double_pointer, 4, 3, 3/2)<<endl;
    return 0;
}
