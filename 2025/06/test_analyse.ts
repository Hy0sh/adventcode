// Analysons l'exemple pas à pas selon l'énoncé

const original = [
    "123 328  51 64 ",
    " 45 64  387 23 ",
    "  6 98  215 314",
    "*   +   *   +  "
];

console.log("=== ORIGINAL ===");
original.forEach((l, i) => console.log(`${i}: "${l}"`));

// Selon l'énoncé, après inversion des nombres:
// "Reading the problems right-to-left one column at a time"

// Je pense que "Cephalopod math" signifie qu'on lit TOUT de droite à gauche
// Y compris les colonnes de problèmes

// L'énoncé dit: "The rightmost problem is 4 + 431 + 623"
// D'où viennent ces nombres?

// Prenons les lignes inversées:
const reversed = [
    "321 823  15 46 ",
    " 54 46  783 32 ",
    "  6 89  512 413"
];

console.log("\n=== NOMBRES INVERSÉS ===");
reversed.forEach((l, i) => console.log(`${i}: "${l}"`));

// Maintenant, lisons colonne par colonne de DROITE à GAUCHE
// Colonne la plus à droite (index le plus élevé):

console.log("\n=== LECTURE COLONNE PAR COLONNE (droite à gauche) ===");

// Position 15 (fin): ' ', ' ', '3' -> un nombre commence!
// Position 14: ' ', '2', '1' 
// Position 13: '6', '3', '4'
// Position 12: ' ', ' ', ' ' -> fin du nombre

// Donc en lisant de haut en bas, on a: ('', '2', '31') + ('6', '3', '4')
// Hmm, ça ne donne pas 4, 431, 623...

// Attendez! Peut-être que chaque ligne donne UN nombre, et on lit ce nombre caractère par caractère de droite à gauche?

console.log("\n=== HYPOTHÈSE: chaque ligne = un nombre, lu de droite à gauche ===");

// Ligne 0 inversée: "321 823  15 46 " -> de droite à gauche, les nombres sont: 46, 15, 823, 321
// Ligne 1 inversée: " 54 46  783 32 " -> de droite à gauche: 32, 783, 46, 54
// Ligne 2 inversée: "  6 89  512 413" -> de droite à gauche: 413, 512, 89, 6

// Maintenant, les problèmes sont définis par les colonnes de séparation
// Problème 1 (le plus à droite): prend le premier nombre de chaque ligne
// 46, 32, 413 ? Non, ça ne donne pas non plus 4, 431, 623

console.log("\n=== HYPOTHÈSE: on lit caractère par caractère ===");

// Peut-être qu'on doit lire caractère par caractère, et chaque colonne de caractères forme un chiffre dans les nombres?

// Lisons position par position de droite à gauche:
// Position 14 (la plus à droite avec un chiffre): 
//   Ligne 0: ' ' 
//   Ligne 1: ' '
//   Ligne 2: '3'
// Donc on a le chiffre '3' pour le 3ème nombre

// Position 13:
//   Ligne 0: '6'
//   Ligne 1: '2'
//   Ligne 2: '1'

// Ah! Donc:
// - Nombre de la ligne 0 (lu de droite à gauche): commence par 6...
// - Nombre de la ligne 1: commence par 2...
// - Nombre de la ligne 2: commence par 1, puis 3...

// Continuons:
// Position 12: ' ', '3', '4' -> les nombres continuent: 6?, 23?, 134?

// Mais l'énoncé dit 4, 431, 623... 

// OHHH! Je pense que je comprends! Les nombres qu'on lit doivent être RE-INVERSÉS!
// Car on les lit de droite à gauche en Cephalopod, donc pour les convertir en normal, il faut les réinverser!

console.log("\n=== SOLUTION: lire de droite à gauche puis réinverser ===");

// Lisons le premier problème (le plus à droite) caractère par caractère:
// Position 14: '', '', '3' -> nombres partiels en Cephalopod: '', '', '3'
// Position 13: '6', '2', '1' -> '', '2', '13' 
// Position 12: ' ', '3', '4' -> '6', '23', '134'
// Position 11: ' ', ' ', ' ' -> fin du problème

// Nombres en Cephalopod (lus de droite à gauche): 6, 23, 134
// Réinverser pour avoir les nombres normaux: 6, 32, 431

// Non, toujours pas ça...

// Réessayons autrement. Les nombres lus sont: '6', '23', '134'
// Si on les réinverse: '6' -> 6, '23' -> 32, '134' -> 431
// Oui mais ça donne 6, 32, 431 et non 4, 431, 623

// Peut-être que je lis dans le mauvais sens? De bas en haut au lieu de haut en bas?
console.log("\n=== Lecture de BAS en HAUT ===");
// Position 14: '3', '', '' -> '3'
// Position 13: '1', '2', '6' -> '31', '2', '6'
// Position 12: '4', '3', ' ' -> '431', '23', '6'

// En Cephalopod: 431, 23, 6
// Réinversés: 134, 32, 6... toujours pas

// Attendez! Peut-être que je ne dois PAS réinverser? Les nombres en Cephalopod SONT déjà inversés
// Donc '431' en Cephalopod = 134 en normal? Non...

// Ou bien: la lecture de BAS en HAUT donne directement les bons nombres?
// 431, 23, 6 -> en les lisant à l'envers: 6, 32, 134... non

// Hmm laissez-moi relire l'énoncé encore...
// "The rightmost problem is 4 + 431 + 623 = 1058"

// Ligne 2 de bas en haut: 413, 512, 89, 6
// Si je lis "413" de droite à gauche chiffre par chiffre: 3, 1, 4 -> 314
// Mais ça c'est juste réinverser ce que j'avais déjà inversé!

console.log("\n=== Peut-être qu'il ne faut PAS inverser les nombres au début? ===");
// Relisons juste de droite à gauche sans inverser les nombres
const origLines = [
    "123 328  51 64 ",
    " 45 64  387 23 ",
    "  6 98  215 314",
];

// De droite à gauche, colonne par colonne
// Position 14 (la plusà droite non-vide):
// Ligne 0: ' '
// Ligne 1: '3'
// Ligne 2: '4'

// De bas en haut: 4, 3, '' -> nombre: 43? ou 4 et 3 séparés?

