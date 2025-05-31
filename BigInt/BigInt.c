#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DIGITS 310 // For ~1024-bit integers (about 308 decimal digits)

typedef struct {
    char digits[MAX_DIGITS];
    int length;
    int is_negative;
    int overflow; // Flag to indicate overflow
} BigInt;

void init_bigint(BigInt *num, const char *value);
void print_bigint(BigInt num);
int compare_abs(BigInt a, BigInt b);
BigInt add_bigint(BigInt a, BigInt b);
BigInt subtract_bigint(BigInt a, BigInt b) ;
BigInt multiply_bigint(BigInt a, BigInt b);

// Function to initialize a BigInt
void init_bigint(BigInt *num, const char *value) {
    num->length = 0;
    num->is_negative = 0;
    num->overflow = 0;

    if (value[0] == '-') {
        num->is_negative = 1;
        value++;
    }

    num->length = strlen(value);
    if (num->length >= MAX_DIGITS) {
        num->overflow = 1;
        return;
    }

    for (int i = 0; i < num->length; i++) {
        num->digits[i] = value[num->length - 1 - i] - '0';
    }
}

// Function to print a BigInt
void print_bigint(BigInt num) {
    if (num.overflow) {
        printf("OVERFLOW\n");
    }

    if (num.is_negative && num.length > 0) printf("-");
    for (int i = num.length - 1; i >= 0; i--) {
        printf("%d", num.digits[i]);
    }
    printf("\n");
}

// Function to compare two BigInts (absolute values only)
// Returns -1 if |a| < |b|, 0 if |a| == |b|, 1 if |a| > |b|
int compare_abs(BigInt a, BigInt b) {
    if (a.length < b.length) return -1;
    if (a.length > b.length) return 1;
    for (int i = a.length - 1; i >= 0; i--) {
        if (a.digits[i] < b.digits[i]) return -1;
        if (a.digits[i] > b.digits[i]) return 1;
    }
    return 0;
}

// Function to add two BigInts
BigInt add_bigint(BigInt a, BigInt b) {
    BigInt result;
    memset(&result, 0, sizeof(BigInt));

    if (a.overflow || b.overflow) {
        result.overflow = 1;
        return result;
    }

    if (a.is_negative == b.is_negative) {
        // Perform addition
        int carry = 0;
        for (int i = 0; i < a.length || i < b.length || carry; i++) {
            if (result.length >= MAX_DIGITS) {
                result.overflow = 1;
                return result;
            }

            int sum = carry;
            if (i < a.length) sum += a.digits[i];
            if (i < b.length) sum += b.digits[i];

            result.digits[result.length++] = sum % 10;
            carry = sum / 10;
        }
        // Remove leading zeros
        while (result.length > 1 && result.digits[result.length - 1] == 0) {
            result.length--;
        }
        result.is_negative = a.is_negative; // Same sign as input numbers
    } else {
        // Perform subtraction if signs are different
        b.is_negative = !b.is_negative;
        result = subtract_bigint(a, b);
        b.is_negative = !b.is_negative;

        // Adjust sign based on the larger absolute value
        if (compare_abs(a, b) >= 0) {
            result.is_negative = a.is_negative;
        } else {
            result.is_negative = b.is_negative;
        }
    }

    return result;
}


// Function to subtract two BigInts
BigInt subtract_bigint(BigInt a, BigInt b) {
    BigInt result;
    memset(&result, 0, sizeof(BigInt));

    if (a.overflow || b.overflow) {
        result.overflow = 1;
        return result;
    }

    if (a.is_negative != b.is_negative) {
        // If signs are different, switch to addition
        b.is_negative = !b.is_negative;
        result = add_bigint(a, b);
        b.is_negative = !b.is_negative;
    } else {
        // Determine the larger number (absolute value)
        int compare = compare_abs(a, b);
        BigInt *minuend = &a, *subtrahend = &b;

        if (compare < 0) {
            result.is_negative = !a.is_negative; // Flip the sign for the result
            minuend = &b;
            subtrahend = &a;
        } else if (compare == 0) {
            result.length = 1;
            result.digits[0] = 0; 
            return result;
        } else {
            result.is_negative = a.is_negative;
        }

        // Perform subtraction
        int borrow = 0;
        for (int i = 0; i < minuend->length; i++) {
            if (result.length >= MAX_DIGITS) {
                result.overflow = 1;
                return result;
            }

            int sub = minuend->digits[i] - borrow;
            if (i < subtrahend->length) sub -= subtrahend->digits[i];

            if (sub < 0) {
                sub += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.digits[result.length++] = sub;
        }

        // Remove leading zeros
        while (result.length > 1 && result.digits[result.length - 1] == 0) {
            result.length--;
        }
    }

    return result;
}


// Function to multiply two BigInts
BigInt multiply_bigint(BigInt a, BigInt b)  {
    BigInt result;
    memset(&result, 0, sizeof(BigInt));

    if (a.overflow || b.overflow) {
        result.overflow = 1;
        return result;
    }

    for (int i = 0; i < a.length; i++) {
        int carry = 0;
        for (int j = 0; j < b.length || carry; j++) {
            if (i + j >= MAX_DIGITS) {
                result.overflow = 1;
                return result;
            }

            int mul = result.digits[i + j] + a.digits[i] * (j < b.length ? b.digits[j] : 0) + carry;
            result.digits[i + j] = mul % 10;
            carry = mul / 10;
        }
    }

    result.length = a.length + b.length;
    if (result.length >= MAX_DIGITS) {
        result.overflow = 1;
        return result;
    }

    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
    }

    result.is_negative = a.is_negative != b.is_negative;

    return result;
}

// Main function to test BigInt
int main() 
{
    BigInt num1, num2, result;

    // Input numbers
    char input1[MAX_DIGITS], input2[MAX_DIGITS];
    printf("Enter the first number: ");
    scanf("%s", input1);
    printf("Enter the second number: ");
    scanf("%s", input2);

    // Initialize BigInts
    init_bigint(&num1, input1);
    init_bigint(&num2, input2);

    // Addition
    result = add_bigint(num1, num2);
    printf("Addition result: ");
    print_bigint(result);

    // Subtraction
    result = subtract_bigint(num1, num2);
    printf("Subtraction result: ");
    print_bigint(result);

    // Multiplication
    result = multiply_bigint(num1, num2);
    printf("Multiplication result: ");
    print_bigint(result);

    return 0;
}
