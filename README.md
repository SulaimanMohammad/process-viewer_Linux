# aise_project

création d'un outil pour montrer tout le processus et les informations liées, nous devons trouver que tous les processus sont en cours d'exécution maintenant.
pour obtenir que l'on puisse accéder au fichier appelé / proc et que le répertoire contient de nombreux fichiers quand ils sont numériques ils représentent les processus et les nombres sont les identifiants des processus
il existe d'autres répertoires qui représentent des informations générales sur le système

la première étape on accède au répertoire /proc et on choisit de lire les noms numériques tpuis on passe le numéro d'identifiant à la fonction process_value (pid) pour calculer les informations liées à ce processus. (man 5 proc)
cette fonction commence par créer deux objets, un à partir de la structure cpuinfo et le second une statue de State 

1- on appelle une fonction read_statFile(proc,pid) dont l'objet de State et le PID 
qui lisent les données existent dans le fichier / proc / pid / state où nous pouvons avoir des données de processus comme le nom, la statue, temp et la mémoire, et nous sauvegardons ces valeurs dans la variable de la structure pour les utiliser après.

2-nous continuons à la fonction  get_process_cpu_usage(cpud,proc, pid);
pour trouver le montant de l'utilisation du processeur pour le processus
la valeur en pourcentage est l'utilisation de la processus par rapport à l'utilisation totale des processeurs
  
   a-l'utilisation du processeur pour un processus spécifique est liée au temps pendant lequel le processus reste en cours d'exécution, nous devons donc trouver l'heure système et l'heure utilisateur du processus cible pour lequel nous pouvons avoir ces données en appelant read_statFile et utiliser la variable enregistrée
utime: temps passé par le processus en mode utilisateur (ticks d'horloge);
stime: temps passé par le processus en mode système (noyau) (ticks d'horloge);

   b-en appelant get_total_cpu_usage, nous pouvons calculer l'utilisation totale de tous les processeurs, cette fonction lit le fichier / proc / stat, qui contient l'utilisation de chaque processeur et pour le total, nous devons lire la première ligne et la somme du nombre représente le utilisation totale dans le système

pour obtenir l'utilisation, nous devons trouver la différence d'utilisation du processeur entre deux moments

   c-nous attendons donc une petite période et nous lisons à nouveau l'utilisation totale du processeur et le nouveau stime, au moment du processus

   d-trouver le temps différent entre ces deux points de mesures et on multiplie par le nombre de cœurs CPU car le processus peut être exécuté sur différents cœurs et l'utilisation totale calculée pour tous les cœurs



3- calculer le temps de processus qui est le temps écoulé depuis le démarrage du processus
nous appelons la fonction comput_process_time
ici il y a un problème, le nombre que nous trouvons dans le / proc / pic / state est lié au processus parent, donc lorsque le PID que nous avons est un enfant du processus principal (pas un enfant de la racine) alors l'heure devrait être calculé d'une manière différente. 
alors pour résoudre ça et avoir le temps:

 a-vérifier si le processus est parent ou enfant (process_isPerent(pid))
 cette fonction ouvre le répertoire / proc / pid / task
ce répertoire contient d'autres répertoires et ils sont classés de manière à ce que le processus parent passe en premier, puis les enfants.

donc cette fonction après avoir accédé au répertoire, elle lit la liste du répertoire à l'intérieur, si le premier nom de répertoire est égal à l'ID de processus qui a appelé la fonction alors le processus est parent, sinon le processus est enfant
donc si la fonction renvoie que le processus est un parent, alors la variable de temps dans / proc / pid / stat peut représenter l'heure du processus

b-si le processus est un enfant, nous devons accéder à un autre fichier pour trouver l'heure, le fichier existe dans / proc / pid / schedset qui contient des statistiques pour les processus individuels, nous prenons donc le premier nombre qui est le temps passé sur le processeur de le processus lui-même même s'il ne s'agit que d'un enfant , et nous devons diviser par 1000000000 comme la référence (1) se réfère.


4- calculer la taille de la mémoire dont nous avons besoin pour utiliser les données "rss" de / proc / pid / stat qui est le nombre de pages que le processus a en mémoire réelle afin d'avoir le nombre total dont nous avons besoin pour multiplier la taille rss * de page (en appelant getpagesize ()) et le résultat est en octet.

pour avoir le perentag, nous devons trouver la taille du processus / l'utilisation totale de la mémoire * 100
donc pour avoir la mémoire totale, nous appelons la fonction (get_total_mem (cpud)) 

5- trouver la quantité de mémoire partagée en appelant la fonction
get_sharedmemory_size dans cette fonction, nous lisons le fichier / proc / pid / statm et le troisième nombre contient la quantité de la taille de la mémoire partagée ou pour être plus précis mais tellement plus difficile à lire les données est la somme des nombres dans le fichier / proc / pid / maps


6-pour calculer la taille de la mémoire virtuelle, nous avons juste besoin des données extraites du fichier / proc / pid / stat où la variable vsize contient le nombre est nécessaire


7-pour connaître le nom du nom d'utilisateur c'est un peu compliqué, on appelle la fonction find_usrname (pid)
  cette fonction fait ce qui suit
 1- nous accédons au fichier / proc / pid / status et nous récupérons la ligne qui contient l'UID qui est la ligne qui représente la référence du numéro d'utilisateur et après avoir obtenu ce numéro, nous recherchons le nom auquel il fait référence
 
 2-après avoir eu l'UID, nous devons entrer dans le répertoire / etc et lire le fichier / etc / passwd dans ce fichier, nous pouvons voir un rapport entre le nom de l'utilisateur et l'UID, donc nous recherchons le numéro UID et sauvegardons le nom



Refe
1- https://lkml.org/lkml/2019/7/24/906
2-https://www.cyberciti.biz/faq/understanding-etcpasswd-file-format/
3-https://en.wikipedia.org/wiki/CPU_time
