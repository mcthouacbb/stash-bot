/*
**    Stash, a UCI chess playing engine developed from scratch
**    Copyright (C) 2019-2025 Morgan Houppin
**
**    Stash is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    Stash is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "psq_table.h"

Scorepair PsqTable[PIECE_NB][SQUARE_NB];

// clang-format off

const Score PieceScores[PHASE_NB][PIECE_NB] = {
    {
        0, PAWN_MG_SCORE, KNIGHT_MG_SCORE, BISHOP_MG_SCORE, ROOK_MG_SCORE, QUEEN_MG_SCORE, 0, 0,
        0, PAWN_MG_SCORE, KNIGHT_MG_SCORE, BISHOP_MG_SCORE, ROOK_MG_SCORE, QUEEN_MG_SCORE, 0, 0
    },
    {
        0, PAWN_EG_SCORE, KNIGHT_EG_SCORE, BISHOP_EG_SCORE, ROOK_EG_SCORE, QUEEN_EG_SCORE, 0, 0,
        0, PAWN_EG_SCORE, KNIGHT_EG_SCORE, BISHOP_EG_SCORE, ROOK_EG_SCORE, QUEEN_EG_SCORE, 0, 0
    }
};

#define S SPAIR

// Square-based Pawn scoring for evaluation
const Scorepair PawnSQT[48] = {
    S(-39,   4), S(-17,   3), S(-36,   6), S(-25,  -5), S(-30,  16), S( 10,  17), S( 17,   6), S(-25, -21),
    S(-37,  -6), S(-38,  -4), S(-22,  -8), S(-24, -10), S(-15,  -1), S(-25,   5), S(  1, -18), S(-23, -18),
    S(-30,   8), S(-29,   1), S(-15, -20), S( -3, -22), S( -2, -22), S( -1, -12), S(-17,  -7), S(-25, -14),
    S(-15,  29), S(-22,   6), S(-12,  -6), S( -2, -28), S(  2, -14), S( 23, -12), S( -9,   0), S(-15,  12),
    S(  8,  45), S(-13,  25), S(  4,   2), S( 22, -10), S( 30,   6), S( 76,  21), S( 35,  38), S( 19,  42),
    S( 96,  -2), S( 30,  -7), S( 52, -10), S( 80, -26), S( 83,  -4), S( 34,  10), S(-25,  18), S(-30,  26)
};

// Square-based piece scoring for evaluation, using a file symmetry
const Scorepair KnightSQT[32] = {
    S( -49,  -42), S( -14,  -38), S( -12,  -16), S(  -7,    0),
    S(  -8,  -25), S(  -2,   -6), S(   7,  -17), S(   6,   -4),
    S(   9,  -34), S(  15,   -7), S(  25,   -8), S(  20,   18),
    S(  19,    0), S(  19,   18), S(  45,   22), S(  31,   35),
    S(  47,   16), S(  47,   13), S(  61,   22), S(  57,   40),
    S(  -4,    7), S(  29,   10), S(  59,   26), S(  55,   25),
    S(  -8,   -2), S( -40,   17), S(  33,    6), S(  32,   32),
    S(-154, -103), S(-116,   11), S(-185,   22), S(   7,   10)
};

const Scorepair BishopSQT[32] = {
    S(  33,  -38), S(  28,  -16), S(  -2,   -6), S(   5,   -9),
    S(  39,  -44), S(  41,  -26), S(  38,  -15), S(  17,    1),
    S(  28,  -10), S(  39,   -4), S(  25,   -7), S(  22,   24),
    S(  12,  -24), S(  22,    4), S(  20,   17), S(  25,   25),
    S(   9,    2), S(  15,   16), S(  27,   18), S(  27,   33),
    S(  42,    8), S(  20,   29), S(  18,   13), S(  34,   11),
    S( -48,    7), S( -55,    0), S(  -1,   15), S( -16,   15),
    S( -45,  -12), S( -71,   21), S(-156,   20), S(-151,   15)
};

const Scorepair RookSQT[32] = {
    S(  -6,  -32), S(  -6,  -23), S(  -6,  -14), S(   0,  -23),
    S( -30,  -29), S( -16,  -28), S(  -7,  -14), S(  -8,  -15),
    S( -27,  -21), S(  -3,  -16), S( -26,   -3), S( -16,   -4),
    S( -21,   -7), S( -18,    4), S( -26,    8), S(  -7,    0),
    S(  -5,   14), S(   5,   18), S(  15,   11), S(  29,    5),
    S( -12,   23), S(  25,   12), S(  23,   12), S(  52,    3),
    S(  10,   24), S(  -5,   24), S(  30,   25), S(  44,   26),
    S(   8,   33), S(   1,   37), S(   1,   37), S(   5,   34)
};

const Scorepair QueenSQT[32] = {
    S(  14,  -67), S(  -1,  -61), S(  17,  -85), S(  25,  -60),
    S(  21,  -63), S(  21,  -57), S(  34,  -55), S(  20,   -6),
    S(  16,  -41), S(  24,  -16), S(  13,   24), S(  10,   19),
    S(  15,   -3), S(  18,   27), S(   1,   33), S(  -9,   65),
    S(  20,   13), S(  -5,   50), S(   8,   32), S( -14,   67),
    S(  10,    6), S(  -7,   36), S( -13,   53), S(  -3,   46),
    S(  -9,    2), S( -40,    6), S( -31,   57), S( -45,   82),
    S( -54,   27), S( -10,   25), S( -21,   47), S( -10,   52)
};

const Scorepair KingSQT[32] = {
    S(  67, -117), S(  65,  -58), S( -20,  -45), S( -26,  -61),
    S(  66,  -51), S(  20,  -19), S( -12,   -5), S( -50,    0),
    S( -35,  -36), S(  14,  -10), S( -22,   15), S( -25,   27),
    S(-163,  -17), S( -77,   15), S( -48,   36), S( -17,   46),
    S(-104,   12), S( -15,   53), S(  22,   65), S(  21,   64),
    S( -43,   24), S(  56,   68), S(  56,   80), S(  58,   67),
    S( -51,  -18), S(   6,   45), S(  38,   59), S(  38,   46),
    S(  24, -239), S(  96,  -41), S(  70,   -3), S(  10,    9)
};

#undef S

// clang-format on

static void psq_table_init_piece(const Scorepair *table, Piece piece) {
    const Scorepair piece_base =
        create_scorepair(PieceScores[MIDGAME][piece], PieceScores[ENDGAME][piece]);

    for (Square square = SQ_A1; square <= SQ_H8; ++square) {
        File qsfile = file_to_queenside(square_file(square));
        Scorepair psqt_entry = piece_base + table[square_rank(square) * 4 + qsfile];

        PsqTable[piece][square] = psqt_entry;
        PsqTable[opposite_piece(piece)][square_flip(square)] = -psqt_entry;
    }
}

static void psq_table_init_pawn(void) {
    const Scorepair pawn_base = create_scorepair(PAWN_MG_SCORE, PAWN_EG_SCORE);

    for (Square square = SQ_A1; square <= SQ_H8; ++square) {
        if (square_rank(square) == RANK_1 || square_rank(square) == RANK_8) {
            PsqTable[WHITE_PAWN][square] = 0;
            PsqTable[BLACK_PAWN][square_flip(square)] = 0;
        } else {
            Scorepair psqt_entry = pawn_base + PawnSQT[square - SQ_A2];

            PsqTable[WHITE_PAWN][square] = psqt_entry;
            PsqTable[BLACK_PAWN][square_flip(square)] = -psqt_entry;
        }
    }
}

void psq_table_init(void) {
    psq_table_init_pawn();
    psq_table_init_piece(KnightSQT, KNIGHT);
    psq_table_init_piece(BishopSQT, BISHOP);
    psq_table_init_piece(RookSQT, ROOK);
    psq_table_init_piece(QueenSQT, QUEEN);
    psq_table_init_piece(KingSQT, KING);
}
