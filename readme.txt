//Projet_PSE_ZBIRI_IDJAHA


Le projet consiste en une messagerie permettant à plusieurs clients de se connecter et d'envoyer des messages. Le nom du client est affiché avant chaque message.


Pour compiler et exécuter le projet, suivez ces étapes :


    Installez GTK en utilisant la commande suivante : sudo apt-get install libgtk-3-dev.
    Compilez le projet avec la commande make.
    Exécutez le projet en utilisant la commande ./interface.


Voici l'architecture du projet :


    Les exécutables se trouvent dans le répertoire "Projet_PSE_ZBIRI_IDJAHA".
    Il y a un programme pour le serveur et un autre pour le client.
	Un autre programme permettant de créer l'interface est dans le meme répertoire
    Le serveur est configuré en local (localhost) et utilise le port 8888, qui ne peut pas être modifié (mais vous pouvez modifier les deux pour vous connecter à un serveur distant).
    Le nombre maximal de clients pouvant se connecter simultanément est limité à 10.
    Une fois que le programme interface.c est exécuté, vous verrez trois boutons :
        "Démarrer serveur" : ce bouton lance l'exécution du serveur (équivalent à ./serv).
        "Démarrer Chat" : ce bouton lance l'exécution du client (équivalent à ./cl).
        "Guide" : ce bouton ouvre le fichier guide.txt qui donne une vision globale de la façon de discuter et d'envoyer des messages dans l'application.


Étant donné que les programmes client, serveur et interface sont indépendants, aucun header n'est nécessaire.


Voici comment cela fonctionne :

    Cliquez d'abord sur "Démarrer serveur".
    Lancez un client en cliquant sur "Démarrer Chat".
    Une fois la connexion établie, un thread est créé pour le client, qui envoie des messages au serveur, lesquels sont ensuite affichés aux autres clients.
    Le premier élément à envoyer par un client est son nom d'utilisateur.
    Chaque fois qu'un nouveau client se connecte, un thread séparé est créé et joint au thread principal du serveur.
    Pour quitter la messagerie, le client appuie sur "quit server", se déconnecte et un message "Client : has left the server" est affiché pour les autres clients connectés.
    Pour vous amuser davantage, veuillez lire le fichier guide.txt en cliquant sur le bouton "Guide".
