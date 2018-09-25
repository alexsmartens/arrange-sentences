#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENTENCE 1024

// Linked list node 
typedef struct list_node {
  char *sentence;
  char *sentence_full;
  struct list_node* next;
} list_node;





// Hash each sentence in one of 26 slots by its first letter
void hash_sentence ( list_node **alphabet, char *sentence_temp,  int len) {
    char first_char = '\0';
    char *first_char_pt;
    // int len = (int) strlen ( sentence_temp );
    list_node *list_node_temp = malloc ( sizeof (list_node) );
    if (!list_node_temp) {
        printf ( "Error occurred during memory allocation\n" ); 
        exit ( 1 );
    }

    // Find the first (capital) letter in a sentence
    for ( int i = 0; i < len; i++ ) {
        if ( sentence_temp[i] >= 'A' && sentence_temp[i] <= 'Z' ){
            first_char = sentence_temp[i];
            break;
        }
    }
    if ( first_char == '\0' ) {
        printf ( "Error: No sentence start found\n" ); 
        exit( 1 );
    }



    // Create a node with corresponding sentence
    // Start sentence from its first capital letter
    first_char_pt = strchr( sentence_temp, first_char);
    list_node_temp->sentence = first_char_pt; 
    // Store full sentence for possibility of further processing or different data representation
    list_node_temp->sentence_full = sentence_temp;
    list_node_temp->next = NULL;


    
    // Adding the node to corresponding linked list in alphabet
    if ( alphabet[first_char - 'A'] ) {
        // Update
        list_node *temp;
        temp = alphabet[first_char - 'A'];
        alphabet[first_char - 'A'] = list_node_temp;
        list_node_temp->next = temp;

    } else {
        alphabet[first_char - 'A'] = list_node_temp;
    }

} 





// Check whether there is a sentence separation mark in the text
char *punctuation_check(char *buf_text){
    // Punctuation marks ponters
    int punct_len = 14;
    char *min = NULL;

    // Identified sentence separation punctuation mark combinations
    char *punctuation_temp[punct_len];
    punctuation_temp[0] = strstr ( buf_text, ". " );
    punctuation_temp[1] = strstr ( buf_text, "? " );
    punctuation_temp[2] = strstr ( buf_text, "! " );
    punctuation_temp[3] = strstr ( buf_text, "./n" );
    punctuation_temp[4] = strstr ( buf_text, "?/n" );
    punctuation_temp[5] = strstr ( buf_text, "!/n" );
    punctuation_temp[6] = strstr ( buf_text, ".\r\n");
    punctuation_temp[7] = strstr ( buf_text, "?\r\n");
    punctuation_temp[8] = strstr ( buf_text, "!\r\n");
    punctuation_temp[9] = strstr ( buf_text, ".\"\r\n");
    punctuation_temp[10] = strstr ( buf_text, "!\"\r\n");
    punctuation_temp[11] = strstr ( buf_text, "?\"\r\n");
    punctuation_temp[12] = strstr ( buf_text, ".\'");
    punctuation_temp[13] = strstr ( buf_text, ".\"" );

    // Find pointer to the end of the shortest sentence in buffer
    for (int i = 0; i < punct_len; i++){
        if ( !min && punctuation_temp[i] ) min = punctuation_temp[i];
        if ( min && punctuation_temp[i] ) {
            if ( punctuation_temp[i] < min ) min = punctuation_temp[i];
        }
    }

    return min;
}





int main ( int argc, char ** argv) {
    // Initial file (for reading)
    FILE *fp;
    char fileName[256];
    // Output file (for writing)
    FILE *fp_mod;
    char fileName_mod[265];
    // Hashtable
    list_node *alphabet[26];
    // Buffers
    char buf[MAX_SENTENCE];
    char buf_text[2 * MAX_SENTENCE];
    buf_text[0] = '\0';
    int max_sentence_real = 0;
    int sentence_count = 0;



    // Check arguments
    if ( argc > 2 ) {
        printf ( "Usage: arrangeSentences file_name(optional)\n" ); 
        exit( 1 );
    }
    // Check optional file name
    if ( argc == 2 ) {
        if ( strlen(argv[1]) > 255 ) {
            printf ( "Error: Too long file name\n" ); 
            exit( 1 );
        }
        strcpy( fileName, argv[1] );
    } else {
        strcpy( fileName, "TheLastQuestion.txt" );
    }



    // Open initial file for reading
    fp = fopen ( fileName, "r" );
    // Open output file for writing
    strcpy( fileName_mod, "modified_" ); // + 9 characters
    strcat( fileName_mod, fileName );
    fp_mod = fopen ( fileName_mod, "w" );
    // Check errors
    if ( !fp || !fp_mod ) {
        perror( "fopen" );
        exit( 1 );
    }


    
    // Initialize alphabet
    for (int i = 0; i < 26; i++) {
        alphabet[i] = NULL;
    }



    // Read data, separate and hash senatnces
    int len = 0;
    char *pt_end;    
    char *sentence_temp;
    while ( fgets (buf, MAX_SENTENCE, fp) != NULL ){  
        strcat ( buf_text, buf );
        while ( ( pt_end = punctuation_check (buf_text) ) ) {
            // New sentence length 
            len = pt_end - buf_text + 1 + 1; // adding two characters for "." and null-terminator
            // Updating actual maximum sentence length and sentence count sentence count
            if ( len > max_sentence_real ) max_sentence_real = len; 
            sentence_count++;
            // Allocating space for the new sentence
            sentence_temp = malloc ( len * sizeof(char) );
            if (!sentence_temp) {
                printf ( "Error occurred during memory allocation\n" ); 
                exit ( 1 );
            }
            strncpy( sentence_temp, buf_text, len - 1 );
            // Updating pointer to point to the next sentence
            pt_end += 1;
            // Deleting already used sentence from the buffer
            strcpy( buf_text, pt_end );

            // Hashing new sentence            
            hash_sentence ( alphabet, sentence_temp , len - 1);
        }

    }



    // Copy hashed and cleaned data to array 
    char arr_sentence[sentence_count][max_sentence_real], temp [max_sentence_real];
    list_node *curr_node;
    list_node *previous_node;
    int idx = 0;
    // int t_idx = 0;
    for ( int i = 0; i < 26; i++ ){
        if ( alphabet[i] ) {
            curr_node = alphabet[i];
            strcpy(arr_sentence[idx], curr_node->sentence);
            idx++;

            while ( curr_node->next ) {
                previous_node = curr_node;
                curr_node = curr_node->next;

                // Free dynamically allocated space
                free(previous_node->sentence_full);
                free(previous_node);

                strcpy(arr_sentence[idx], curr_node->sentence);
                idx++;
            }
            
            // Free dynamically allocated space
            free(curr_node->sentence_full);
            free(curr_node);
        }
    }



    // Sort hashed strings
    for (int i = 0; i < sentence_count; i++){
        for (int j = i + 1; j < sentence_count; j++){
            if (strcmp(arr_sentence[i], arr_sentence[j]) > 0) {
                    strcpy(temp, arr_sentence[i]);
                    strcpy(arr_sentence[i], arr_sentence[j]);
                    strcpy(arr_sentence[j], temp);
            }
            
            // Compare chosen string to stings with the same first letter only
            if (arr_sentence[i][0] !=  arr_sentence[j][0]) {
                break;
            }
        }
    }



    // Write data 
    char new_line = '\n';
    for (int i = 0; i < sentence_count; i++ ){
        if ( 1 != fwrite( arr_sentence[i], strlen(arr_sentence[i]) * sizeof (char), 1,  fp_mod ) ) {
                printf ( "Error occurred while writing to the file\n" ); 
                exit ( 1 );
            }
        if ( 1 != fwrite( &new_line, sizeof (char), 1,  fp_mod ) ) {
            printf ( "Error occurred while writing to the file\n" ); 
            exit ( 1 );
        }
    }



    // Closing the files
    if ( fclose ( fp ) || fclose ( fp_mod )) {
        printf ( "Error occurred while closing the files\n" ); 
        exit( 1 );
    }

    printf("The input sentences have been successfully arranged in \"%s\" file\n", fileName_mod);
    
    return 0;
}