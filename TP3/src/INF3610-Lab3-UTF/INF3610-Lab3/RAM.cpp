///////////////////////////////////////////////////////////////////////////////
//
//	RAM.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "RAM.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
RAM::RAM( sc_module_name name, const char *fileName, unsigned long RAMSize, bool verbose )
: sc_module(name), m_RAMSize(RAMSize), m_verbose(verbose)
{
	// Variable
	FILE * file;
	size_t result;

	// Initialisation
	m_ptrRAM = NULL;
	
	// Allocation de la mémoire
	m_ptrRAM = malloc( m_RAMSize );
	
	// On valide si la mémoire est allouée
	if ( m_ptrRAM == NULL )
	{
		cout << "Erreur lors de l'allocation de " << m_RAMSize << " octets" << endl;
		
		// Fin de la simulation
		m_bError = true;
	}

	// On initialise la mémoire
	memset(m_ptrRAM, 0, m_RAMSize);
	
	// Ouverture du fichier
	file = fopen( fileName, "rb" );
	
	// Validation
	if (file == 0)
	{
		cout << "Erreur lors de l'ouverture du fichier d'entré" << endl;
		
		// Fin de la simulation
		m_bError = true;
	}
	else
	{
		// On va chercher la taille du fichier
		fseek( file , 0 , SEEK_END );
		m_size = ftell(file);
		rewind(file);
				
		// On s'assure que le fichier n'est pas trop volumineux
		if ( m_size > m_RAMSize )
		{
			cout << "Fichier d'entrée trop volumineux pour la RAM" << endl;
			
			// Fin de la simulation
			m_bError = true;
		}
		else
		{
			// On remplie le contenu de la mémoire avec le fichier d'entré
			result = fread( m_ptrRAM, 1, m_size, file );
			
			// On valide que tout s'est bien déroulé
			if (result != m_size)
			{
				cout << "Erreur lors de l'écriture du fichier d'entrée en mémoire" << endl;
				
				// Fin de la simulation
				m_bError = true;
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
RAM::~RAM()
{
	// On vide la mémoire
	if ( m_ptrRAM != NULL )
		free( m_ptrRAM );
}


///////////////////////////////////////////////////////////////////////////////
//
//	Read
//
///////////////////////////////////////////////////////////////////////////////
unsigned int RAM::Read(unsigned int addr)
{
	// Variable
	unsigned int uiData = 0;
	
	if ( addr >= m_RAMSize )
	{
		cout << "Lecture hors de la plage de la RAM" << endl;
		
		// Fin de la simulation
		sc_stop();
	}
	else
	{
		// On avertie l'usager
		if ( ( addr >= m_size ) && m_verbose )
			cout << "Lecture de la mémoire hors de la plage du fichier d'entrée" << endl;
			
		// On copie le contenu de la mémoire
		memcpy( &uiData, (void*)((unsigned int)(m_ptrRAM) + addr), 4);
	}
	
	// Retourne le contenu de la mémoire
	return uiData;
}
