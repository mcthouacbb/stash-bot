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
    S(-39,   7), S(-18,   6), S(-35,   6), S(-25,  -8), S(-29,  13), S(  9,  11), S( 17,   4), S(-24, -19),
    S(-37,  -4), S(-38,  -2), S(-21,  -7), S(-23, -10), S(-13,  -3), S(-26,   4), S(  1, -18), S(-23, -18),
    S(-30,  11), S(-30,   4), S(-15, -17), S( -2, -23), S(  0, -22), S( -2, -12), S(-17,  -7), S(-25, -15),
    S(-17,  32), S(-23,  10), S(-11,  -3), S( -2, -26), S(  4, -16), S( 21, -13), S(-11,   0), S(-15,  12),
    S(  8,  47), S(-12,  26), S( 10,   2), S( 20, -11), S( 37,   6), S( 72,  20), S( 30,  34), S( 16,  41),
    S( 97,  -5), S( 31,  -9), S( 52,  -8), S( 81, -24), S( 81,  -3), S( 36,  12), S(-24,  14), S(-38,  19)
};

// Square-based piece scoring for evaluation, using a file symmetry
const Scorepair KnightSQT[32] = {
    S( -49,  -43), S( -12,  -38), S( -11,  -18), S(  -6,    1),
    S(  -7,  -24), S(  -1,   -5), S(   8,  -17), S(   7,   -4),
    S(  10,  -33), S(  15,   -7), S(  25,   -8), S(  21,   17),
    S(  19,    1), S(  19,   16), S(  46,   20), S(  31,   33),
    S(  46,   16), S(  46,   13), S(  59,   21), S(  55,   39),
    S(  -5,    9), S(  29,   11), S(  57,   25), S(  54,   25),
    S(  -8,   -2), S( -40,   18), S(  33,    6), S(  32,   31),
    S(-153, -102), S(-116,   12), S(-185,   22), S(   7,   11)
};

const Scorepair BishopSQT[32] = {
    S(  32,  -38), S(  28,  -18), S(  -2,   -7), S(   4,  -11),
    S(  39,  -44), S(  41,  -27), S(  37,  -16), S(  16,    1),
    S(  28,  -11), S(  39,   -4), S(  25,   -7), S(  22,   23),
    S(  12,  -24), S(  22,    4), S(  21,   17), S(  25,   25),
    S(   8,    1), S(  15,   16), S(  27,   18), S(  26,   33),
    S(  42,    8), S(  20,   29), S(  19,   13), S(  35,   10),
    S( -49,    7), S( -54,    2), S(   0,   17), S( -15,   14),
    S( -45,  -11), S( -71,   21), S(-156,   20), S(-151,   15)
};

const Scorepair RookSQT[32] = {
    S(  -7,  -30), S(  -7,  -23), S(  -6,  -15), S(  -1,  -23),
    S( -31,  -29), S( -16,  -29), S(  -8,  -14), S(  -8,  -16),
    S( -28,  -21), S(  -4,  -17), S( -26,   -4), S( -16,   -4),
    S( -22,   -7), S( -19,    4), S( -26,    8), S(  -8,    0),
    S(  -6,   13), S(   5,   18), S(  15,   11), S(  29,    4),
    S( -13,   23), S(  26,   12), S(  24,   13), S(  54,    3),
    S(  12,   24), S(  -3,   25), S(  33,   25), S(  48,   27),
    S(   7,   31), S(   0,   35), S(   1,   36), S(   5,   33)
};

const Scorepair QueenSQT[32] = {
    S(  14,  -68), S(  -2,  -62), S(  18,  -87), S(  25,  -63),
    S(  21,  -63), S(  21,  -58), S(  34,  -57), S(  20,   -7),
    S(  15,  -41), S(  24,  -16), S(  13,   23), S(  10,   18),
    S(  15,   -5), S(  18,   28), S(   1,   34), S( -10,   66),
    S(  20,   12), S(  -5,   49), S(   7,   34), S( -15,   69),
    S(  10,    6), S(  -7,   35), S( -12,   52), S(  -3,   49),
    S(  -9,    2), S( -40,    7), S( -30,   59), S( -45,   83),
    S( -54,   27), S( -10,   24), S( -21,   47), S( -11,   52)
};

const Scorepair KingSQT[32] = {
    S(  62, -121), S(  55,  -61), S( -27,  -42), S( -30,  -56),
    S(  63,  -57), S(  14,  -24), S( -11,   -8), S( -43,   -2),
    S( -37,  -43), S(  15,  -16), S( -17,   11), S( -15,   22),
    S(-162,  -20), S( -75,    9), S( -44,   32), S( -15,   41),
    S(-104,   11), S( -14,   50), S(  24,   61), S(  22,   61),
    S( -43,   30), S(  56,   74), S(  57,   83), S(  59,   68),
    S( -51,  -10), S(   6,   60), S(  39,   72), S(  38,   54),
    S(  24, -239), S(  96,  -38), S(  70,    1), S(  10,   12)
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
