#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// Function to draw and save the bar graph
void draw_bar_graph(const long int* data, int num_bars, const char* filename)
{
	// Dimension of the image
	const long int width = 4000;
	const long int height = 3000;

	// Margins
	const long int margin_left = 80;
	const long int margin_bottom = 80;
	const long int margin_top = 40;
	const long int margin_right = 40;

	// Drawing area
	const long int graph_width = width - margin_left - margin_right;
	const long int graph_height = height - margin_top - margin_bottom;

	// Bar width and spacing
	// This will ensure that bars are equally distributed across the
	// axis, and no space is left
	const long int bar_width = graph_width / (num_bars * 2);
	const long int bar_spacing = bar_width; // Equal space between bars

	// Create surface and context
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cairo_t* cr = cairo_create(surface);

	// Background
	cairo_set_source_rgb(cr, 1, 1, 1); // white
	cairo_paint(cr);

	// Draw axes. Note: Top peft of canvas is (0,0) here. x increases
	// to right, y increases down.
	cairo_set_source_rgb(cr, 0, 0, 0); // black
	cairo_set_line_width(cr, 2); // axis line width
	cairo_move_to(cr, margin_left, margin_top); // leaves some gap in top (left and above)
	cairo_line_to(cr, margin_left, height - margin_bottom); // Y-axis //////////////////////////////////////////////////
	cairo_line_to(cr, width - margin_right, height - margin_bottom); // X-axis
	cairo_stroke(cr);

	// Scaling of y axis
	const long int max_value = num_bars; // As the data is currently sorted (1-100), max /////////////////////////////////
	// value is total number of bars
	const long int scale = graph_height / max_value;

	// Draw bars. Note: for cairo, the (0,0) starts from the top left
	// corner. So for drawing a bar graph, x-axis of that graph wont
	// be affected, but y-axis pointer will be inverted, meaning more
	// the value of y, lower the graph.
	for (int i = 0; i < num_bars; ++i) {
		long int bar_height = data[i] * scale; // set bar height for each bar
		long int x = margin_left + i * (bar_width + bar_spacing); // set the x position of each bar, top left corner
		long int y = height - (margin_bottom + bar_height); // set the y position of each bar ///////////////////////////////////////////
		cairo_set_source_rgb(cr, 0.2, 0.4, 0.8); // blue (200, 400, 800)
		cairo_rectangle(cr, x, y, bar_width, bar_height);
		cairo_fill(cr);
	}

	// Write to file
	cairo_surface_write_to_png(surface, filename);

	// Cleanup
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}



// Function to Bubble sort an array
void bubble_sort(long int arr[], int n) {
    long int counter = 1;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j]; 
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                // Now for saving the png file
                char snum[5]; // used in integer to string conversion
                sprintf(snum, "%04ld", counter); // converts int to string, stores in snum, adds zero padding of 4 example 0001
                char filename[50] = "./bar_graphs/bar_graph_";
                strcat(filename, snum);
                strcat(filename, ".png");
                draw_bar_graph(arr, n, filename);
                printf("Created %ld\n", counter);
                counter += 1;
            }
        }
    }
}



// Function to sort an array using heap sort
void heapify(long int arr[], int heap_size, int i, int full_n, long int *counter) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap_size && arr[left] > arr[largest])
        largest = left;

    if (right < heap_size && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        long int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        // Save bar graph after each swap using full_n
        char snum[5];
        sprintf(snum, "%04ld", *counter);
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, full_n, filename);
        printf("Created %ld\n", *counter);
        (*counter)++;

        heapify(arr, heap_size, largest, full_n, counter);
    }
}

void heap_sort(long int arr[], int n) {
    long int counter = 1;
    int full_n = n;

    // Build the heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, full_n, &counter);

    // Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        // Move root to end
        long int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Save full array state after swap
        char snum[5];
        sprintf(snum, "%04ld", counter);
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, full_n, filename);
        printf("Created %ld\n", counter);
        counter++;

        // Heapify the reduced heap
        heapify(arr, i, 0, full_n, &counter);
    }
}



// Function to sort an array using selection sort
void selection_sort(long int arr[], int N) {
    long int counter = 1;
    // Start with the whole array as unsored and one by
  	// one move boundary of unsorted subarray towards right
    for (int i = 0; i < N - 1; i++) {

        // Find the minimum element in unsorted array
        int min_idx = i;
        for (int j = i + 1; j < N; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // Swap the found minimum element with the first
        // element in the unsorted part
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
        
        // Save full array state after swap
        char snum[5];
        sprintf(snum, "%04ld", counter);
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, N, filename);
        printf("Created %ld\n", counter);
        counter++;
    }
}





// Function to sort an array using insertion sort
void insertion_sort(long int arr[], int n) {
    long int counter = 1;

    for (int i = 1; i < n; i++) {
        long int key = arr[i];
        int j = i - 1;

        // Shift elements to make room for key
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];

            // Save state after each shift
            char snum[5];
            sprintf(snum, "%04ld", counter);
            char filename[50] = "./bar_graphs/bar_graph_";
            strcat(filename, snum);
            strcat(filename, ".png");
            draw_bar_graph(arr, n, filename);
            printf("Created %ld\n", counter);
            counter++;

            j--;
        }

        arr[j + 1] = key;

        // Save state after inserting the key in its correct position
        char snum[5];
        sprintf(snum, "%04ld", counter);
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, n, filename);
        printf("created %ld\n", counter);
        counter++;
    }
}


// Function to sort an array using merge sort 
void merge(long int arr[], int l, int m, int r, int n, long int *counter) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    long int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    // Merge back into arr[]
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }

        // Save graph after placing each element
        char snum[5];
        sprintf(snum, "%04ld", *counter);
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, n, filename);
        printf("Created %ld\n", *counter);
        (*counter)++;

        k++;
    }

    // Copy any remaining elements of L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;

        // Save graph
        char snum[5];
        sprintf(snum, "%04ld", *counter);
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, n, filename);
        printf("Created %ld\n", *counter);
        (*counter)++;
    }

    // Copy any remaining elements of R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;

        // Save graph
        char snum[5];
        sprintf(snum, "%04ld", *counter);
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, n, filename);
        printf("Created %ld\n", *counter);
        (*counter)++;
    }
}

void merge_sort_recursive(long int arr[], int l, int r, int n, long int *counter) {
    if (l < r) {
        int m = l + (r - l) / 2;

        merge_sort_recursive(arr, l, m, n, counter);
        merge_sort_recursive(arr, m + 1, r, n, counter);
        merge(arr, l, m, r, n, counter);
    }
}

void merge_sort(long int arr[], int n) {
    long int counter = 1;
    merge_sort_recursive(arr, 0, n - 1, n, &counter);
}


// Function to sort an array using Shell sort
void shell_sort(long int array[], int n)
{
	long int counter = 1;
	// Rearrange elements at each n/2, n/4, n/8, ... intervals
	for (int interval = n / 2; interval > 0; interval /= 2) {
		for (int i = interval; i < n; i += 1) {
			int temp = array[i];
			int j;
			for (j = i; j >= interval && array[j - interval] > temp; j -= interval) {
				array[j] = array[j - interval];
				char snum[5];
				sprintf(snum, "%04ld", counter);
				char filename[50] = "./bar_graphs/bar_graph_";
				strcat(filename, snum);
				strcat(filename, ".png");
				draw_bar_graph(array, n, filename);
				printf("created %ld\n", counter);
				counter++;
			}
			array[j] = temp;
			char snum[5];
			sprintf(snum, "%04ld", counter);
			char filename[50] = "./bar_graphs/bar_graph_";
			strcat(filename, snum);
			strcat(filename, ".png");
			draw_bar_graph(array, n, filename);
			printf("created %ld\n", counter);
			counter++;
		}
	}
}



// Function to cocktail shaker sort an array
void cocktail_shaker_sort(long int arr[], int n) {
    long int counter = 1;
    int start = 0;
    int end = n - 1;
    int swapped = 1;

    while (swapped) {
        swapped = 0;

        // Left to right
        for (int i = start; i < end; ++i) {
            if (arr[i] > arr[i + 1]) {
                long int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;

                // Save frame
                char snum[5];
                sprintf(snum, "%04ld", counter);
                char filename[50] = "./bar_graphs/bar_graph_";
                strcat(filename, snum);
                strcat(filename, ".png");
                draw_bar_graph(arr, n, filename);
                printf("Created %ld\n", counter);
                counter++;
            }
        }

        if (!swapped)
            break;

        swapped = 0;
        end--;

        // Right to left
        for (int i = end - 1; i >= start; --i) {
            if (arr[i] > arr[i + 1]) {
                long int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;

                // Save frame
                char snum[5];
                sprintf(snum, "%04ld", counter);
                char filename[50] = "./bar_graphs/bar_graph_";
                strcat(filename, snum);
                strcat(filename, ".png");
                draw_bar_graph(arr, n, filename);
                printf("Created %ld\n", counter);
                counter++;
            }
        }
        start++;
    }
}



// Function to use gnome sort to sort an array
void gnome_sort(long int arr[], int n) {
    long int counter = 1;
    int index = 0;

    while (index < n) {
        if (index == 0)
            index++;
        if (arr[index] >= arr[index - 1]) {
            index++;
        } else {
            long int temp = arr[index];
            arr[index] = arr[index - 1];
            arr[index - 1] = temp;

            // Save frame
            char snum[5];
            sprintf(snum, "%04ld", counter);
            char filename[50] = "./bar_graphs/bar_graph_";
            strcat(filename, snum);
            strcat(filename, ".png");
            draw_bar_graph(arr, n, filename);
            printf("Created %ld\n", counter);
            counter++;

            index--;
        }
    }
}

// Function to sort an array using comb sort
void comb_sort(long int arr[], int n) {
    long int counter = 1;
    int gap = n;
    const float shrink = 1.3;  // Common shrink factor
    int sorted = 0;

    while (!sorted) {
        // Update the gap value for a next comb
        gap = (int)(gap / shrink);
        if (gap <= 1) {
            gap = 1;
            sorted = 1; // Assume sorted
        }

        for (int i = 0; i + gap < n; i++) {
            if (arr[i] > arr[i + gap]) {
                long int temp = arr[i];
                arr[i] = arr[i + gap];
                arr[i + gap] = temp;
                sorted = 0;

                // Save frame
                char snum[5];
                sprintf(snum, "%04ld", counter);
                char filename[50] = "./bar_graphs/bar_graph_";
                strcat(filename, snum);
                strcat(filename, ".png");
                draw_bar_graph(arr, n, filename);
                printf("Created %ld\n", counter);
                counter++;
            }
        }
    }
}



void cycle_sort(long int arr[], int n) {
    long int counter = 1;

    for (int cycle_start = 0; cycle_start < n - 1; cycle_start++) {
        long int item = arr[cycle_start];
        int pos = cycle_start;

        // Find position where we put the item
        for (int i = cycle_start + 1; i < n; i++) {
            if (arr[i] < item)
                pos++;
        }

        // If item is already in correct position
        if (pos == cycle_start)
            continue;

        // Skip duplicates
        while (item == arr[pos]) {
            pos++;
        }

        // Swap the item to its correct position
        if (pos != cycle_start) {
            long int temp = item;
            item = arr[pos];
            arr[pos] = temp;

            // Visualize
            char snum[5];
            sprintf(snum, "%04ld", counter++);
            char filename[50] = "./bar_graphs/bar_graph_";
            strcat(filename, snum);
            strcat(filename, ".png");
            draw_bar_graph(arr, n, filename);
            printf("Created %ld\n", counter - 1);
        }

        // Rotate the rest of the cycle
        while (pos != cycle_start) {
            pos = cycle_start;
            for (int i = cycle_start + 1; i < n; i++) {
                if (arr[i] < item)
                    pos++;
            }

            while (item == arr[pos]) {
                pos++;
            }

            if (item != arr[pos]) {
                long int temp = item;
                item = arr[pos];
                arr[pos] = temp;

                // Visualize
                char snum[5];
                sprintf(snum, "%04ld", counter++);
                char filename[50] = "./bar_graphs/bar_graph_";
                strcat(filename, snum);
                strcat(filename, ".png");
                draw_bar_graph(arr, n, filename);
                printf("Created %ld\n", counter - 1);
            }
        }
    }
}










// Tim sort

#define RUN 32

void insertion_sort_tim(long int arr[], int left, int right, long int* counter, int n) {
    for (int i = left + 1; i <= right; i++) {
        long int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;

            // Visualize
            char snum[5];
            sprintf(snum, "%04ld", (*counter));
            char filename[50] = "./bar_graphs/bar_graph_";
            strcat(filename, snum);
            strcat(filename, ".png");
            draw_bar_graph(arr, n, filename);
            printf("Created %ld\n", (*counter));
            (*counter)++;
        }
        arr[j + 1] = temp;

        // Visualize insert
        char snum[5];
        sprintf(snum, "%04ld", (*counter));
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, n, filename);
        printf("Created %ld\n", (*counter));
        (*counter)++;
    }
}

void merge_tim(long int arr[], int l, int m, int r, long int* counter, int n) {
    int len1 = m - l + 1, len2 = r - m;
    long int* left = (long int*)malloc(len1 * sizeof(long int));
    long int* right = (long int*)malloc(len2 * sizeof(long int));

    for (int i = 0; i < len1; i++) left[i] = arr[l + i];
    for (int i = 0; i < len2; i++) right[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }

        // Visualize
        char snum[5];
        sprintf(snum, "%04ld", (*counter));
        char filename[50] = "./bar_graphs/bar_graph_";
        strcat(filename, snum);
        strcat(filename, ".png");
        draw_bar_graph(arr, n, filename);
        printf("Created %ld\n", (*counter));
        (*counter)++;
    }

    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];

    free(left);
    free(right);

    // Final merge state
    char snum[5];
    sprintf(snum, "%04ld", (*counter));
    char filename[50] = "./bar_graphs/bar_graph_";
    strcat(filename, snum);
    strcat(filename, ".png");
    draw_bar_graph(arr, n, filename);
    printf("Created %ld\n", (*counter));
    (*counter)++;
}

void timsort(long int arr[], int n) {
    long int counter = 1;

    // Step 1: Sort individual subarrays of size RUN
    for (int i = 0; i < n; i += RUN) {
        int right = (i + RUN - 1 < n - 1) ? (i + RUN - 1) : (n - 1);
        insertion_sort_tim(arr, i, right, &counter, n);
    }

    // Step 2: Merge subarrays
    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = (left + size - 1 < n - 1) ? (left + size - 1) : (n - 1);
            int right = (left + 2 * size - 1 < n - 1) ? (left + 2 * size - 1) : (n - 1);

            if (mid < right)
                merge_tim(arr, left, mid, right, &counter, n);
        }
    }
}





// Wiki sort
// Rotate arr[l..m] and arr[m+1..r] in-place: arr[l..r] -> arr[m+1..r] + arr[l..m]
void rotate(long int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    while (n1 > 0 && n2 > 0) {
        if (n1 <= n2) {
            // Swap first n1 elements of right with n1 elements of left
            for (int i = 0; i < n1; i++) {
                long int temp = arr[l + i];
                arr[l + i] = arr[m + 1 + i];
                arr[m + 1 + i] = temp;
            }
            l += n1;
            m += n1;
            n2 -= n1;
        } else {
            // Swap last n2 elements of left with n2 elements of right
            for (int i = 0; i < n2; i++) {
                long int temp = arr[m - n2 + 1 + i];
                arr[m - n2 + 1 + i] = arr[m + 1 + i];
                arr[m + 1 + i] = temp;
            }
            r -= n2;
            m -= n2;
            n1 -= n2;
        }
    }
}

// In-place stable merge arr[l..m] and arr[m+1..r]
void inplace_merge(long int arr[], int l, int m, int r, long int* counter, int n) {
    int i = l;
    int j = m + 1;

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) {
            i++;
        } else {
            // Element at j is smaller, rotate to put it before i
            int start = i;
            int mid = j;
            int end = j;
            // Find how many elements on right are smaller than arr[i]
            while (end + 1 <= r && arr[end + 1] < arr[i]) {
                end++;
            }

            rotate(arr, start, mid - 1, end);

            int shift = end - mid + 1;
            i += shift + 1;
            m += shift;
            j = end + 1;

            // Save frame after rotation (merge step)
            char snum[5];
            sprintf(snum, "%04ld", (*counter));
            char filename[50] = "./bar_graphs/bar_graph_";
            strcat(filename, snum);
            strcat(filename, ".png");
            draw_bar_graph(arr, n, filename);
            printf("Created %ld\n", (*counter));
            (*counter)++;
        }
    }
}

void inplace_merge_sort(long int arr[], int l, int r, long int* counter, int n) {
    if (l < r) {
        int m = l + (r - l) / 2;
        inplace_merge_sort(arr, l, m, counter, n);
        inplace_merge_sort(arr, m + 1, r, counter, n);
        inplace_merge(arr, l, m, r, counter, n);
    }
}

void wiki_sort(long int arr[], int n) {
    long int counter = 1;
    inplace_merge_sort(arr, 0, n - 1, &counter, n);
}




// Function to create video out of given images
void create_video()
{
	printf("\nCreating video...\n");
	int output = system("ffmpeg -framerate 100 -i ./bar_graphs/bar_graph_%04d.png output.avi");

	if (output == 0)
		printf("Done!");
	else
		printf("Error!");
}

// Function to shuffle given array. Its called Fisher Yates algorithm
void shuffle_array(long int arr[], int n)
{
	srand(time(NULL)); // Seed the random number generator
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1); // Generate a random index between 0 and i
		// Swap arr[i] with arr[j]
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}

// Function to generate a sorted array. Will be shuffled and sorted later.
void generate_perfect_array(long int arr[], int size)
{
	for (int i = 1; i <= size; i++) {
		arr[i - 1] = i;
	}
}

// Function to print the whole array
void print_array(long int arr[], int size)
{
	for (int i = 0; i < size; i++) {
		printf("%ld ", arr[i]);
	}
}

// The main function
int main()
{
	long int array[100];
	int choice;

	generate_perfect_array(array, 100);
	shuffle_array(array, 100);
	// print_array(array, 100);

	printf("Welcome to the sorting algorithm visualiser\n0. Visualise the images in the directory\n");
	printf("1. Bubble Sort\n2. Heap Sort\n3. Selection sort\n");
	printf("4. Insertion sort\n5. Merge sort\n6. Shell sort\n7. Cocktail Shaker sort\n");
	printf("8. Gnome sort\n9. Comb sort\n10. Cycle sort\n11. Tim sort\n12. Wiki sort\n");
	printf("Enter the sorting algorithm that you want to visualise : ");
	scanf("%d", &choice);
	
	switch (choice)
	{
		case 0:
			create_video();
			return 0;
		case 1:
			bubble_sort(array, 100);
			break;
		case 2:
			heap_sort(array, 100);
			break;
		case 3:
			selection_sort(array, 100);
			break;
		case 4:
			insertion_sort(array, 100);
			break;
		case 5:
			merge_sort(array, 100);
			break;
		case 6:
			shell_sort(array, 100);
			break;
		case 7:
			cocktail_shaker_sort(array, 100);
			break;
		case 8:
			gnome_sort(array, 100);
			break;
		case 9:
			comb_sort(array, 100);
			break;
		case 10:
			cycle_sort(array, 100);
			break;
		case 11:
			timsort(array, 100);
			break;
		case 12:
			wiki_sort(array, 100);
			break;
		default:
			printf("You have entered wrong value\n");
			return main(); // Recursively calling main is not ideal; consider using a loop instead
	}

	create_video();
}
