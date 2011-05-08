/*
 ============================================================================
 Name        : kdgsubcalc.c
 Author      : Yvan Janssens
 Version     :
 Copyright   : (c) Yvan Jansses 2011 (INF105B)
 Description : Subnetberekeningen, the easy way.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void pause() {
	char buffer[128] = {0};
	printf("[return] to exit...");
	fflush(stdin);
	fgets(buffer, 128, stdin);
	fgets(buffer, 128, stdin);
	printf("\n");
}

int b8(int pos128, int pos64, int pos32, int pos16, int pos8, int pos4, int pos2, int pos1) {
	if (pos128 > 1) pos128 = 1;
	if (pos128 < 0) pos128 = 0;
	if (pos64 > 1) pos64 = 1;
	if (pos64 < 0) pos64 = 0;
	if (pos32 > 1) pos32 = 1;
	if (pos32 < 0) pos32 = 0;
	if (pos16 > 1) pos16 = 1;
	if (pos16 < 0) pos16 = 0;
	if (pos8 > 1) pos8 = 1;
	if (pos8 < 0) pos8 = 0;
	if (pos4 > 1) pos4 = 1;
	if (pos4 < 0) pos4 = 0;
	if (pos2 > 1) pos2 = 1;
	if (pos2 < 0) pos2 = 0;
	if (pos1 > 1) pos1 = 1;
	if (pos1 < 0) pos1 = 0;
	return pos128*128 + pos64*64 + pos32*32 + pos16*16 + pos8*8 + pos4*4 + pos2*2 + pos1;
}

int bpos(int n, int pos) {
	char bit[8] = {0};
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		bit[i] = n & 1;
		n >>= 1;
	}
	return bit[pos];
}

int main(void) {
	int octets[5] = {0,0,0,0,0};
	int byteval[8] = {0};
	int netname[5] = {0,0,0,0,0};
	int firsthost[5] = {0,0,0,0,0};
	int lasthost[5] = {0,0,0,0,0};
	char opgaveinvoer[64] = {0};
	char octet[4] = {0};
	int counter = 0;
	int position = 0;
	int octetcounter = 0;
	int subnetcount = 0;
	int temp = 0;

	printf("kdgsubcalc - aid for netarch labo\n");

	fflush(stdin);
	printf("assignment ? ");
	fgets(opgaveinvoer, 64, stdin);
	opgaveinvoer[strlen(opgaveinvoer) - 1] = 0x00;

	for (counter = 0; counter < strlen(opgaveinvoer); counter ++) {
		if (strncmp(&opgaveinvoer[counter],".", 1) == 0) {
			position = 0;
			octets[octetcounter] = atoi(octet);
			octetcounter ++;
			octet[0] = 0;octet[1] = 0;octet[2] = 0;octet[3] = 0;
		} else if (strncmp(&opgaveinvoer[counter], "/", 1) == 0) {
			position = 0;
			octets[octetcounter] = atoi(octet);
			octetcounter ++;
			octet[0] = 0;octet[1] = 0;octet[2] = 0;octet[3] = 0;
		} else {
			octet[position] = opgaveinvoer[counter];
			position ++;
		}
	}
	octets[octetcounter] = atoi(octet);

	printf("subnet ? ");
	fflush(stdin);
	scanf("%d", &subnetcount);
	if (octets[4] < 8) {
		// Nog niets
	} else if (octets[4] < 16) {
		netname[0] = octets[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = 0;

		temp = subnetcount;
		temp <<= 8 - (octets[4] - 8);

		netname[1] = temp;
		netname[2] = 0;
		netname[3] = 0;

		if (netname[1] > 255)
		{
			netname[0] += (netname[1] >> 8); // xxxx xxxx 0000 0000 --> xxxx xxxx

			netname[1] %= 256;
		}

		firsthost[0] = netname[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = bpos(netname[1], counter);
		for (counter = 0; counter < octets[4] - 8; counter ++)
			byteval[counter] = 0;
		firsthost[1] = netname[1] | b8(byteval[7],byteval[6],byteval[5],byteval[4],byteval[3],byteval[2],byteval[1],byteval[0]);
		firsthost[2] = 0;
		firsthost[3] = 1;
		lasthost[0] = netname[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = bpos(netname[1], counter);
		for (counter = 0; counter <  8 -(octets[4] - 8); counter ++)
			byteval[counter] = 1;
		lasthost[1] = netname[0] | b8(byteval[7],byteval[6],byteval[5],byteval[4],byteval[3],byteval[2],byteval[1],byteval[0]);
		lasthost[2] = 0xFF;
		lasthost[3] = 0xFE;
	} else if (octets[4] < 24) {
		netname[0] = octets[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = 0;

		temp = subnetcount;
		temp <<= 8 - (octets[4] - 16);


		netname[1] = octets[1];
		netname[2] = temp;
		netname[3] = 0;

		if (temp > 255) {
			netname[1] += (netname[2] >> 8);
			netname[2] %= 256;
		}

		firsthost[0] = netname[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = bpos(netname[2], counter);
		for (counter = 0; counter < octets[4] - 16; counter ++)
			byteval[counter] = 0;
		firsthost[1] = netname[1];
		firsthost[2] = b8(byteval[7],byteval[6],byteval[5],byteval[4],byteval[3],byteval[2],byteval[1],byteval[0]);
		firsthost[3] = 1;
		lasthost[0] = netname[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = bpos(netname[2], counter);
		for (counter = 0; counter < 8 - (octets[4] - 16); counter ++)
			byteval[counter] = 1;
		lasthost[1] = netname[1];
		lasthost[2] = b8(byteval[7],byteval[6],byteval[5],byteval[4],byteval[3],byteval[2],byteval[1],byteval[0]);
		lasthost[3] = 0xFE;
	} else if (octets[4] < 32) {
		netname[0] = octets[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = 0;

		temp = subnetcount;
		temp <<= 8 - (octets[4] - 24);

		netname[1] = octets[1];
		netname[2] = octets[2];
		netname[3] = temp;

		if (temp > 255) {
			netname[2] += (netname[3] >> 8);
			netname[3] %= 256;
		}

		firsthost[0] = netname[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = bpos(netname[3], counter);
		for (counter = 0; counter < 8 - ( octets[4] - 24 ); counter ++)
			byteval[counter] = 0;
		firsthost[1] = netname[1];
		firsthost[2] = netname[2];
		firsthost[3] = b8(byteval[7],byteval[6],byteval[5],byteval[4],byteval[3],byteval[2],byteval[1],byteval[0]) + 1;
		lasthost[0] = netname[0];
		for (counter = 0; counter < 8; counter ++)
			byteval[counter] = bpos(netname[3], counter);
		for (counter = 0; counter < 8 -( octets[4] - 24); counter ++)
			byteval[counter] = counter == 0 ? 0 : 1;
		lasthost[1] = netname[1];
		lasthost[2] = netname[2];
		lasthost[3] = b8(byteval[7],byteval[6],byteval[5],byteval[4],byteval[3],byteval[2],byteval[1],byteval[0]);
	}
	printf("network name: %d.%d.%d.%d \n", netname[0], netname[1], netname[2], netname[3]);
	printf("first       : %d.%d.%d.%d \n", firsthost[0], firsthost[1], firsthost[2], firsthost[3]);
	printf("last        : %d.%d.%d.%d \n", lasthost[0], lasthost[1], lasthost[2], lasthost[3]);
	pause();
	return EXIT_SUCCESS;
}
