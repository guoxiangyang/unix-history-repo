/*
 * DviChar.c
 *
 * Map DVI (ditroff output) character names to
 * font indexes and back
 */

# include   "DviChar.h"

# define allocHash()	((DviCharNameHash *) malloc (sizeof (DviCharNameHash)))

struct map_list {
	struct map_list	*next;
	DviCharNameMap	*map;
};

static struct map_list	*world;

static int	standard_maps_loaded = 0;
static void	load_standard_maps ();
static int	hash_name ();

DviCharNameMap *
DviFindMap (encoding)
	char	*encoding;
{
	struct map_list	*m;

	if (!standard_maps_loaded)
		load_standard_maps ();
	for (m = world; m; m=m->next)
		if (!strcmp (m->map->encoding, encoding))
			return m->map;
	return 0;
}

void
DviRegisterMap (map)
	DviCharNameMap	*map;
{
	struct map_list	*m;
	static dispose_hash(), compute_hash();

	if (!standard_maps_loaded)
		load_standard_maps ();
	for (m = world; m; m = m->next)
		if (!strcmp (m->map->encoding, map->encoding))
			break;
	if (!m) {
		m = (struct map_list *) malloc (sizeof *m);
		m->next = world;
		world = m;
	}
	dispose_hash (map);
	m->map = map;
	compute_hash (map);
}

static
dispose_hash (map)
	DviCharNameMap	*map;
{
	DviCharNameHash	**buckets;
	DviCharNameHash	*h, *next;
	int		i;

	buckets = map->buckets;
	for (i = 0; i < DVI_HASH_SIZE; i++) {
		for (h = buckets[i]; h; h=next) {
			next = h->next;
			free (h);
		}
	}
}

static int
hash_name (name)
	char	*name;
{
	int	i = 0;

	while (*name)
		i = (i << 1) ^ *name++;
	if (i < 0)
		i = -i;
	return i;
}

static
compute_hash (map)
	DviCharNameMap	*map;
{
	DviCharNameHash	**buckets;
	int		c, s, i;
	DviCharNameHash	*h;

	buckets = map->buckets;
	for (i = 0; i < DVI_HASH_SIZE; i++)
		buckets[i] = 0;
	for (c = 0; c < DVI_MAP_SIZE; c++)
		for (s = 0; s < DVI_MAX_SYNONYMS; s++) {
			if (!map->dvi_names[c][s])
				break;
			i = hash_name (map->dvi_names[c][s]) % DVI_HASH_SIZE;
			h = allocHash ();
			h->next = buckets[i];
			buckets[i] = h;
			h->name = map->dvi_names[c][s];
			h->position = c;
		}
	
}

int
DviCharIndex (map, name)
	DviCharNameMap	*map;
	char		*name;
{
	int		c, s, i;
	DviCharNameHash	*h;

	i = hash_name (name) % DVI_HASH_SIZE;
	for (h = map->buckets[i]; h; h=h->next)
		if (!strcmp (h->name, name))
			return h->position;
	return -1;
}

static DviCharNameMap ISO8859_1_map = {
	"iso8859-1",
	0,
{
{	0,		/* 0 */},
{	0,		/* 1 */},
{	0,		/* 2 */},
{	0,		/* 3 */},
{	0,		/* 4 */},
{	0,		/* 5 */},
{	0,		/* 6 */},
{	0,		/* 7 */},
{	0,		/* 8 */},
{	0,		/* 9 */},
{	0,		/* 10 */},
{	0,		/* 11 */},
{	0,		/* 12 */},
{	0,		/* 13 */},
{	0,		/* 14 */},
{	0,		/* 15 */},
{	0,		/* 16 */},
{	0,		/* 17 */},
{	0,		/* 18 */},
{	0,		/* 19 */},
{	0,		/* 20 */},
{	0,		/* 21 */},
{	0,		/* 22 */},
{	0,		/* 23 */},
{	0,		/* 24 */},
{	0,		/* 25 */},
{	0,		/* 26 */},
{	0,		/* 27 */},
{	0,		/* 28 */},
{	0,		/* 29 */},
{	0,		/* 30 */},
{	0,		/* 31 */},
{	0,		/* 32 */},
{	"!",		/* 33 */},
{	"\"",		/* 34 */},
{	"#",		/* 35 */},
{	"$",		/* 36 */},
{	"%",		/* 37 */},
{	"&",		/* 38 */},
{	"'",		/* 39 */},
{	"(",		/* 40 */},
{	")",		/* 41 */},
{	"*",		/* 42 */},
{	"+",		/* 43 */},
{	",",		/* 44 */},
{	"\\-",		/* 45 */},
{	".",		/* 46 */},
{	"/","sl",	/* 47 */},
{	"0",		/* 48 */},
{	"1",		/* 49 */},
{	"2",		/* 50 */},
{	"3",		/* 51 */},
{	"4",		/* 52 */},
{	"5",		/* 53 */},
{	"6",		/* 54 */},
{	"7",		/* 55 */},
{	"8",		/* 56 */},
{	"9",		/* 57 */},
{	":",		/* 58 */},
{	";",		/* 59 */},
{	"<",		/* 60 */},
{	"=","eq",	/* 61 */},
{	">",		/* 62 */},
{	"?",		/* 63 */},
{	"@",		/* 64 */},
{	"A",		/* 65 */},
{	"B",		/* 66 */},
{	"C",		/* 67 */},
{	"D",		/* 68 */},
{	"E",		/* 69 */},
{	"F",		/* 70 */},
{	"G",		/* 71 */},
{	"H",		/* 72 */},
{	"I",		/* 73 */},
{	"J",		/* 74 */},
{	"K",		/* 75 */},
{	"L",		/* 76 */},
{	"M",		/* 77 */},
{	"N",		/* 78 */},
{	"O",		/* 79 */},
{	"P",		/* 80 */},
{	"Q",		/* 81 */},
{	"R",		/* 82 */},
{	"S",		/* 83 */},
{	"T",		/* 84 */},
{	"U",		/* 85 */},
{	"V",		/* 86 */},
{	"W",		/* 87 */},
{	"X",		/* 88 */},
{	"Y",		/* 89 */},
{	"Z",		/* 90 */},
{	"[",		/* 91 */},
{	"\\",		/* 92 */},
{	"]",		/* 93 */},
{	"^","a^","ha"	/* 94 */},
{	"_",		/* 95 */},
{	"`",		/* 96 */},
{	"a",		/* 97 */},
{	"b",		/* 98 */},
{	"c",		/* 99 */},
{	"d",		/* 100 */},
{	"e",		/* 101 */},
{	"f",		/* 102 */},
{	"g",		/* 103 */},
{	"h",		/* 104 */},
{	"i",		/* 105 */},
{	"j",		/* 106 */},
{	"k",		/* 107 */},
{	"l",		/* 108 */},
{	"m",		/* 109 */},
{	"n",		/* 110 */},
{	"o",		/* 111 */},
{	"p",		/* 112 */},
{	"q",		/* 113 */},
{	"r",		/* 114 */},
{	"s",		/* 115 */},
{	"t",		/* 116 */},
{	"u",		/* 117 */},
{	"v",		/* 118 */},
{	"w",		/* 119 */},
{	"x",		/* 120 */},
{	"y",		/* 121 */},
{	"z",		/* 122 */},
{	"{",		/* 123 */},
{	"|","or","ba"	/* 124 */},
{	"}",		/* 125 */},
{	"~","a~","ap","ti"	/* 126 */},
{	0,		/* 127 */},
{	0,		/* 128 */},
{	0,		/* 129 */},
{	0,		/* 130 */},
{	0,		/* 131 */},
{	0,		/* 132 */},
{	0,		/* 133 */},
{	0,		/* 134 */},
{	0,		/* 135 */},
{	0,		/* 136 */},
{	0,		/* 137 */},
{	0,		/* 138 */},
{	0,		/* 139 */},
{	0,		/* 140 */},
{	0,		/* 141 */},
{	0,		/* 142 */},
{	0,		/* 143 */},
{	0,		/* 144 */},
{	0,		/* 145 */},
{	0,		/* 146 */},
{	0,		/* 147 */},
{	0,		/* 148 */},
{	0,		/* 149 */},
{	0,		/* 150 */},
{	0,		/* 151 */},
{	0,		/* 152 */},
{	0,		/* 153 */},
{	0,		/* 154 */},
{	0,		/* 155 */},
{	0,		/* 156 */},
{	0,		/* 157 */},
{	0,		/* 158 */},
{	0,		/* 159 */},
{	0,		/* 160 */},
{	"r!", "\241",	/* 161 */},
{	"ct", "\242",	/* 162 */},
{	"Po", "\243",	/* 163 */},
{	"Cs", "\244",	/* 164 */},
{	"Ye", "\245",	/* 165 */},
{	"bb", "\246",	/* 166 */},
{	"sc", "\247",	/* 167 */},
{	"ad", "\250",	/* 168 */},
{	"co", "\251",	/* 169 */},
{	"Of", "\252",	/* 170 */},
{	"Fo", "\253",	/* 171 */},
{	"no", "\254",	/* 172 */},
{	"-", "hy", "\255"      	/* 173 */},
{	"rg", "\256",	/* 174 */},
{	"a-", "\257",	/* 175 */},
{	"de", "\260",	/* 176 */},
{	"+-", "\261",	/* 177 */},
{	"S2", "\262",	/* 178 */},
{	"S3", "\263",	/* 179 */},
{	"aa", "\264",	/* 180 */},
/* Omit *m here; we want *m to match the other greek letters in the
   symbol font. */
{	"\265",		/* 181 */},
{	"ps", "\266",	/* 182 */},
{	"md", "\267",	/* 183 */},
{	"ac", "\270",	/* 184 */},
{	"S1", "\271",	/* 185 */},
{	"Om", "\272",	/* 186 */},
{	"Fc", "\273",	/* 187 */},
{	"14", "\274",	/* 188 */},
{	"12", "\275",	/* 189 */},
{	"34", "\276",	/* 190 */},
{	"r?", "\277",	/* 191 */},
{	"`A", "\300",	/* 192 */},
{	"'A", "\301",	/* 193 */},
{	"^A", "\302",	/* 194 */},
{	"~A", "\303",	/* 195 */},
{	":A", "\304",	/* 196 */},
{	"oA", "\305",	/* 197 */},
{	"AE", "\306",	/* 198 */},
{	",C", "\307",	/* 199 */},
{	"`E", "\310",	/* 200 */},
{	"'E", "\311",	/* 201 */},
{	"^E", "\312",	/* 202 */},
{	":E", "\313",	/* 203 */},
{	"`I", "\314",	/* 204 */},
{	"'I", "\315",	/* 205 */},
{	"^I", "\316",	/* 206 */},
{	":I", "\317",	/* 207 */},
{	"-D", "\320",	/* 208 */},
{	"~N", "\321",	/* 209 */},
{	"`O", "\322",	/* 210 */},
{	"'O", "\323",	/* 211 */},
{	"^O", "\324",	/* 212 */},
{	"~O", "\325",	/* 213 */},
{	":O", "\326",	/* 214 */},
{	"mu", "\327",	/* 215 */},
{	"/O", "\330",	/* 216 */},
{	"`U", "\331",	/* 217 */},
{	"'U", "\332",	/* 218 */},
{	"^U", "\333",	/* 219 */},
{	":U", "\334",	/* 220 */},
{	"'Y", "\335",	/* 221 */},
{	"TP", "\336",	/* 222 */},
{	"ss", "\337",	/* 223 */},
{	"`a", "\340",	/* 224 */},
{	"'a", "\341",	/* 225 */},
{	"^a", "\342",	/* 226 */},
{	"~a", "\343",	/* 227 */},
{	":a", "\344",	/* 228 */},
{	"oa", "\345",	/* 229 */},
{	"ae", "\346",	/* 230 */},
{	",c", "\347",	/* 231 */},
{	"`e", "\350",	/* 232 */},
{	"'e", "\351",	/* 233 */},
{	"^e", "\352",	/* 234 */},
{	":e", "\353",	/* 235 */},
{	"`i", "\354",	/* 236 */},
{	"'i", "\355",	/* 237 */},
{	"^i", "\356",	/* 238 */},
{	":i", "\357",	/* 239 */},
{	"Sd", "\360",	/* 240 */},
{	"~n", "\361",	/* 241 */},
{	"`o", "\362",	/* 242 */},
{	"'o", "\363",	/* 243 */},
{	"^o", "\364",	/* 244 */},
{	"~o", "\365",	/* 245 */},
{	":o", "\366",	/* 246 */},
{	"di", "\367",	/* 247 */},
{	"/o", "\370",	/* 248 */},
{	"`u", "\371",	/* 249 */},
{	"'u", "\372",	/* 250 */},
{	"^u", "\373",	/* 251 */},
{	":u", "\374",	/* 252 */},
{	"'y", "\375",	/* 253 */},
{	"Tp", "\376",	/* 254 */},
{	":y", "\377",	/* 255 */},
}};

static DviCharNameMap Adobe_Symbol_map = {
	"adobe-fontspecific",
	1,
{
{	0,	/* 0 */},
{	0,	/* 1 */},
{	0,	/* 2 */},
{	0,	/* 3 */},
{	0,	/* 4 */},
{	0,	/* 5 */},
{	0,	/* 6 */},
{	0,	/* 7 */},
{	0,	/* 8 */},
{	0,	/* 9 */},
{	0,	/* 10 */},
{	0,	/* 11 */},
{	0,	/* 12 */},
{	0,	/* 13 */},
{	0,	/* 14 */},
{	0,	/* 15 */},
{	0,	/* 16 */},
{	0,	/* 17 */},
{	0,	/* 18 */},
{	0,	/* 19 */},
{	0,	/* 20 */},
{	0,	/* 21 */},
{	0,	/* 22 */},
{	0,	/* 23 */},
{	0,	/* 24 */},
{	0,	/* 25 */},
{	0,	/* 26 */},
{	0,	/* 27 */},
{	0,	/* 28 */},
{	0,	/* 29 */},
{	0,	/* 30 */},
{	0,	/* 31 */},
{	0,	/* 32 */},
{	"!", 	/* 33 */},
{	"fa", 	/* 34 */},
{	"#", "sh", 	/* 35 */},
{	"te", 	/* 36 */},
{	"%", 	/* 37 */},
{	"&", 	/* 38 */},
{	"st",	/* 39 */},
{	"(", 	/* 40 */},
{	")", 	/* 41 */},
{	"**", 	/* 42 */},
{	"+", "pl", 	/* 43 */},
{	",", 	/* 44 */},
{	"\\-", "mi", 	/* 45 */},
{	".", 	/* 46 */},
{	"/", "sl", 	/* 47 */},
{	"0", 	/* 48 */},
{	"1", 	/* 49 */},
{	"2", 	/* 50 */},
{	"3", 	/* 51 */},
{	"4", 	/* 52 */},
{	"5", 	/* 53 */},
{	"6", 	/* 54 */},
{	"7", 	/* 55 */},
{	"8", 	/* 56 */},
{	"9", 	/* 57 */},
{	":", 	/* 58 */},
{	";", 	/* 59 */},
{	"<", 	/* 60 */},
{	"=", "eq", 	/* 61 */},
{	">", 	/* 62 */},
{	"?", 	/* 63 */},
{	"=~", 	/* 64 */},
{	"*A", 	/* 65 */},
{	"*B", 	/* 66 */},
{	"*X", 	/* 67 */},
{	"*D", 	/* 68 */},
{	"*E", 	/* 69 */},
{	"*F", 	/* 70 */},
{	"*G", 	/* 71 */},
{	"*Y", 	/* 72 */},
{	"*I", 	/* 73 */},
{	0,	/* 74 */},
{	"*K", 	/* 75 */},
{	"*L", 	/* 76 */},
{	"*M", 	/* 77 */},
{	"*N", 	/* 78 */},
{	"*O", 	/* 79 */},
{	"*P", 	/* 80 */},
{	"*H", 	/* 81 */},
{	"*R", 	/* 82 */},
{	"*S", 	/* 83 */},
{	"*T", 	/* 84 */},
{	0, 	/* 85 */},
{	"ts", 	/* 86 */},
{	"*W", 	/* 87 */},
{	"*C", 	/* 88 */},
{	"*Q", 	/* 89 */},
{	"*Z", 	/* 90 */},
{	"[", "lB", 	/* 91 */},
{	"tf", "3d", 	/* 92 */},
{	"]", "rB", 	/* 93 */},
{	"pp", 	/* 94 */},
{	"_", 	/* 95 */},
{	"rn",	/* 96 */},
{	"*a", 	/* 97 */},
{	"*b", 	/* 98 */},
{	"*x", 	/* 99 */},
{	"*d", 	/* 100 */},
{	"*e", 	/* 101 */},
{	"*f", 	/* 102 */},
{	"*g", 	/* 103 */},
{	"*y", 	/* 104 */},
{	"*i", 	/* 105 */},
{	0,	/* 106 */},
{	"*k", 	/* 107 */},
{	"*l", 	/* 108 */},
{ "*m", "\265", /* 109 */},
{	"*n", 	/* 110 */},
{	"*o", 	/* 111 */},
{	"*p", 	/* 112 */},
{	"*h", 	/* 113 */},
{	"*r", 	/* 114 */},
{	"*s", 	/* 115 */},
{	"*t", 	/* 116 */},
{	"*u", 	/* 117 */},
{	0,	/* 118 */},
{	"*w", 	/* 119 */},
{	"*c", 	/* 120 */},
{	"*q", 	/* 121 */},
{	"*z", 	/* 122 */},
{	"lC", "{", 	/* 123 */},
{	"ba", "or", "|", 	/* 124 */},
{	"rC", "}", 	/* 125 */},
{	"ap", 	/* 126 */},
{	0,	/* 127 */},
{	0,	/* 128 */},
{	0,	/* 129 */},
{	0,	/* 130 */},
{	0,	/* 131 */},
{	0,	/* 132 */},
{	0,	/* 133 */},
{	0,	/* 134 */},
{	0,	/* 135 */},
{	0,	/* 136 */},
{	0,	/* 137 */},
{	0,	/* 138 */},
{	0,	/* 139 */},
{	0,	/* 140 */},
{	0,	/* 141 */},
{	0,	/* 142 */},
{	0,	/* 143 */},
{	0,	/* 144 */},
{	0,	/* 145 */},
{	0,	/* 146 */},
{	0,	/* 147 */},
{	0,	/* 148 */},
{	0,	/* 149 */},
{	0,	/* 150 */},
{	0,	/* 151 */},
{	0,	/* 152 */},
{	0,	/* 153 */},
{	0,	/* 154 */},
{	0,	/* 155 */},
{	0,	/* 156 */},
{	0,	/* 157 */},
{	0,	/* 158 */},
{	0,	/* 159 */},
{	0,	/* 160 */},
{	"*U",	/* 161 */},
{	"fm", 	/* 162 */},
{	"<=", 	/* 163 */},
{	"f/", 	/* 164 */},
{	"if", 	/* 165 */},
{	0,	/* 166 */},
{	"CL",	/* 167 */},
{	"DI",	/* 168 */},
{	"HE",	/* 169 */},
{	"SP",	/* 170 */},
{	"<>",	/* 171 */},
{	"<-", 	/* 172 */},
{	"ua", 	/* 173 */},
{	"->", 	/* 174 */},
{	"da", 	/* 175 */},
{ "de", "\260", /* 176 */},
{ "+-", "\261", /* 177 */},
{	"sd",	/* 178 */},
{	">=", 	/* 179 */},
{ "mu", "\327", /* 180 */},
{	"pt", 	/* 181 */},
{	"pd", 	/* 182 */},
{	"bu", 	/* 183 */},
{ "di", "\367", /* 184 */},
{	"!=", 	/* 185 */},
{	"==", 	/* 186 */},
{	"~=", "~~", 	/* 187 */},
{	0,	/* 188 */},
{	0,	/* 189 */},
{	0,	/* 190 */},
{	"CR",	/* 191 */},
{	"Ah", 	/* 192 */},
{	"Im", 	/* 193 */},
{	"Re", 	/* 194 */},
{	0,	/* 195 */},
{	"c*", 	/* 196 */},
{	"c+", 	/* 197 */},
{	"es", 	/* 198 */},
{	"ca", 	/* 199 */},
{	"cu", 	/* 200 */},
{	"sp", 	/* 201 */},
{	"ip", 	/* 202 */},
{	0,	/* 203 */},
{	"sb", 	/* 204 */},
{	"ib", 	/* 205 */},
{	"mo", 	/* 206 */},
{	"nm", 	/* 207 */},
{	"/_",	/* 208 */},
{	"gr", 	/* 209 */},
{	"rg",	/* 210 */},
{	"co",	/* 211 */},
{	"tm",	/* 212 */},
{	0,	/* 213 */},
{	"sr", 	/* 214 */},
{	0,	/* 215 */},
{ "no", "\254", /* 216 */},
{	"AN", 	/* 217 */},
{	"OR", 	/* 218 */},
{	"hA",	/* 219 */},
{	"lA",	/* 220 */},
{	"uA",	/* 221 */},
{	"rA",	/* 222 */},
{	"dA",	/* 223 */},
{	0,	/* 224 */},
{	"la", 	/* 225 */},
{	0,	/* 226 */},
{	0,	/* 227 */},
{	0,	/* 228 */},
{	0,	/* 229 */},
{	"parenlefttp", 		/* 230 */},
{	"parenleftex", 		/* 231 */},
{	"parenleftbt", 		/* 232 */},
{	"bracketlefttp", "lc", 	/* 233 */},
{	"bracketleftex", 	/* 234 */},
{	"bracketleftbt", "lf",	/* 235 */},
{	"bracelefttp", "lt",	/* 236 */},
{	"braceleftmid", "lk", 	/* 237 */},
{	"braceleftbt", "lb",	/* 238 */},
{	"bracerightex", "braceleftex", "bv",	/* 239 */},
{	0,	/* 240 */},
{	"ra", 	/* 241 */},
{	"is", 	/* 242 */},
{	0,	/* 243 */},
{	0,	/* 244 */},
{	0,	/* 245 */},
{	"parenrighttp", 	/* 246 */},
{	"parenrightex", 	/* 247 */},
{	"parenrightbt", 	/* 248 */},
{	"bracketrighttp", "rc",	/* 249 */},
{	"bracketrightex", 	/* 250 */},
{	"bracketrightbt", "rf",	/* 251 */},
{	"bracerighttp", "rt"	/* 252 */},
{	"bracerightmid", "rk"	/* 253 */},
{	"bracerightbt", "rb"	/* 254 */},
{	0,	/* 255 */},
}};


static void
load_standard_maps ()
{
	standard_maps_loaded = 1;
	DviRegisterMap (&ISO8859_1_map);
	DviRegisterMap (&Adobe_Symbol_map);
}

