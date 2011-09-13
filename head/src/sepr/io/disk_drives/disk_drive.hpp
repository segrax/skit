
class cDisk_Drive : public cDrive {

public:
				 cDisk_Drive( std::string pName, cSepr *pSepr ) :  cDrive( pName, pSepr ) {

				 }

	virtual void		 headMove(int pDirection) = 0;
	virtual void		 diskRotateStart() = 0;
	virtual void		 diskRotateStop() = 0;

};
