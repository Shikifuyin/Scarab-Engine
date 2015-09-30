<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/CodeDoc/CodeDoc.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Modules, CodeDoc
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
require_once( "CodeDocGenerator.php" );

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

	// Null ID
define( "CODEDOC_ID_NULL", 1 );

	// Meta types
define( "CODEDOC_METATYPE_UNION",     "union" );
define( "CODEDOC_METATYPE_STRUCT",    "struct" );
define( "CODEDOC_METATYPE_CLASS",     "class" );
define( "CODEDOC_METATYPE_INTERFACE", "interface" );

	// Scope Protection
define( "CODEDOC_PROTECTION_PUBLIC",    "public" );
define( "CODEDOC_PROTECTION_PROTECTED", "protected" );
define( "CODEDOC_PROTECTION_PRIVATE",   "private" );

	// Derivation methods
define( "CODEDOC_DERIVMETHOD_PUBLIC",  "public" );
define( "CODEDOC_DERIVMETHOD_PRIVATE", "private" );
define( "CODEDOC_DERIVMETHOD_VIRTUAL_PUBLIC",  "virtual_public" );
define( "CODEDOC_DERIVMETHOD_VIRTUAL_PRIVATE", "virtual_private" );

	// XReferences stuff
define( "CODEDOC_XREFID_UNRESOLVED", 0xffffffff );

define( "CODEDOC_XREFKIND_TYPE", 	 "Type" );
define( "CODEDOC_XREFKIND_DEFINE", 	 "Define" );
define( "CODEDOC_XREFKIND_MACRO", 	 "Macro" );
define( "CODEDOC_XREFKIND_ENUM", 	 "Enum" );
define( "CODEDOC_XREFKIND_TYPEDEF",  "Typedef" );
define( "CODEDOC_XREFKIND_VARIABLE", "Variable" );
define( "CODEDOC_XREFKIND_FUNCTION", "Function" );

	// CodeDoc events
define( "CODEDOC_DISPLAY_ROOT", 		  "DisplayRoot" );
define( "CODEDOC_DISPLAY_GETTINGSTARTED", "DisplayGettingStarted" );
define( "CODEDOC_DISPLAY_CONVENTIONS", 	  "DisplayConventions" );
define( "CODEDOC_DISPLAY_FRAMEWORK", 	  "DisplayFramework" );
define( "CODEDOC_DISPLAY_MANUAL",    	  "DisplayManual" );
define( "CODEDOC_DISPLAY_REFERENCE", 	  "DisplayReference" );
define( "CODEDOC_DISPLAY_DIRECTORY",      "DisplayDirectory" );
define( "CODEDOC_DISPLAY_FILE", 		  "DisplayFile" );
define( "CODEDOC_DISPLAY_DEFINE", 		  "DisplayDefine" );
define( "CODEDOC_DISPLAY_MACRO", 		  "DisplayMacro" );
define( "CODEDOC_DISPLAY_NAMESPACE",	  "DisplayNamespace" );
define( "CODEDOC_DISPLAY_TYPE", 		  "DisplayType" );
define( "CODEDOC_DISPLAY_ENUM", 		  "DisplayEnum" );
define( "CODEDOC_DISPLAY_TYPEDEF",	 	  "DisplayTypedef" );
define( "CODEDOC_DISPLAY_VARIABLE", 	  "DisplayVariable" );
define( "CODEDOC_DISPLAY_FUNCTION", 	  "DisplayFunction" );

define( "CODEDOC_EVENT_ADMIN_GENERATE", "AdminGenerate" );
define( "CODEDOC_EVENT_ADMIN_UPDATE",   "AdminUpdate" );

/////////////////////////////////////////////////////////////////////////////////
// The CodeDocManager class
class CodeDocManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( CodeDocManager::$m_hInstance == NULL )
            CodeDocManager::$m_hInstance = new CodeDocManager();
        return CodeDocManager::$m_hInstance;
    }
    protected static $m_hInstance;
    
    // Constructor / Destructor
    public function __construct()
    {
		$this->_GenerateTables();
    }
	public function __destruct()
    {
		// nothing to do
	}

	// Documentation Generator
	public function Cleanup()
	{
		$hSQL = SQLManager::GetSingleton();

		// Drop all tables
		$hSQL->DropTable( "_PHP_CodeDoc_CrossedReferences" );
		$hSQL->DropTable( "_PHP_CodeDoc_CrossedLinks" );
		$hSQL->DropTable( "_PHP_CodeDoc_Functions" );
		$hSQL->DropTable( "_PHP_CodeDoc_Variables" );
		$hSQL->DropTable( "_PHP_CodeDoc_Typedefs" );
		$hSQL->DropTable( "_PHP_CodeDoc_Enums" );
		$hSQL->DropTable( "_PHP_CodeDoc_Types" );
		$hSQL->DropTable( "_PHP_CodeDoc_Namespaces" );
		$hSQL->DropTable( "_PHP_CodeDoc_Macros" );
		$hSQL->DropTable( "_PHP_CodeDoc_Defines" );
		$hSQL->DropTable( "_PHP_CodeDoc_Files" );
		$hSQL->DropTable( "_PHP_CodeDoc_Directories" );
		$hSQL->DropTable( "_PHP_CodeDoc_Documentation" );

		// Regenerate blank tables
		$this->_GenerateTables();
	}

	public function GenerateStructure()
	{
		$this->Cleanup();

		$hCodeDocGen = new CodeDocGenerator();
		$hCodeDocGen->GenerateStructure();
	}
	public function ImportContent()
	{
		$hCodeDocGen = new CodeDocGenerator();
		$hCodeDocGen->ImportContent();
	}
	/*public function ExportContent()
	{
		$hCodeDocGen = new CodeDocGenerator();
		$hCodeDocGen->ExportContent();
	}*/
	
	// Documentation operations
	public function UpdateDocumentation( $strName, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Documentation", array( "ShortDesc"  => $strShortDesc,
														    "DocContent" => $strDocContent ),
					   "Name='?'", array( $strName ) );
	}
	public function GetDocumentationInfos( $strName )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Documentation", NULL,
					   "Name='?'", array( $strName ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrDocumentationRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrDocumentationRow;
	}

	// Directories operations
	public function CreateDirectory( $iParentDirID, $strName, $strPath, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = "";
		if ( $iParentDirID != CODEDOC_ID_NULL ) {
			$arrDirectoryInfos = $this->GetDirectoryInfos( $iParentDirID );
			$strScopedNameID = $arrDirectoryInfos["ScopedNameID"];
		}
		$strScopedNameID .= "/$strName";

		$hSQL->Insert( "_PHP_CodeDoc_Directories", array( "ParentDirID"  => $iParentDirID,
														  "ScopedNameID" => $strScopedNameID,
														  "Name" 		 => $strName,
														  "Path" 		 => $strPath,
														  "ShortDesc" 	 => $strShortDesc,
														  "DocContent" 	 => $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateDirectory( $iDirectoryID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Directories", array( "ShortDesc"  => $strShortDesc,
														  "DocContent" => $strDocContent ),
					   "DirectoryID=?", array( $iDirectoryID ) );
	}
	public function ListDirectories( $iParentDirID, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "DirectoryID" );
		$hSQL->Select( "_PHP_CodeDoc_Directories", $arrKeys,
					   "ParentDirID=?", array( $iParentDirID ),
					   "Name" );

		$arrDirectories = array();
		$arrDirectoryRow = $hSQL->FetchRow();
		while( $arrDirectoryRow != NULL ) {
			// Never give access to null elements
			if ( $arrDirectoryRow["DirectoryID"] == CODEDOC_ID_NULL ) {
				$arrDirectoryRow = $hSQL->FetchRow();
				continue;
			}

			if ( $bGetInfos )
				array_push( $arrDirectories, $arrDirectoryRow );
			else
				array_push( $arrDirectories, $arrDirectoryRow["DirectoryID"] );
			$arrDirectoryRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrDirectories;
	}
	public function GetDirectoryInfos( $iDirectoryID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Directories", NULL,
					   "DirectoryID=?", array( $iDirectoryID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrDirectoryRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrDirectoryRow;
	}
	public function GetDirectoryID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Directories", array( "DirectoryID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrDirectoryRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrDirectoryRow["DirectoryID"];
	}

	// Files operations
	public function CreateFile( $iParentDirID, $strName, $strPath, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = "";
		if ( $iParentDirID != CODEDOC_ID_NULL ) {
			$arrDirectoryInfos = $this->GetDirectoryInfos( $iParentDirID );
			$strScopedNameID = $arrDirectoryInfos["ScopedNameID"];
		}
		$strScopedNameID .= "/$strName";
		
		$hSQL->Insert( "_PHP_CodeDoc_Files", array( "ParentDirID"  => $iParentDirID,
													"ScopedNameID" => $strScopedNameID,
													"Name" 		   => $strName,
													"Path" 		   => $strPath,
													"ShortDesc"    => $strShortDesc,
													"DocContent"   => $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateFile( $iFileID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Files", array( "ShortDesc"  => $strShortDesc,
													"DocContent" => $strDocContent ),
					   "FileID=?", array( $iFileID ) );
	}
	public function ListFiles( $iParentDirID, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "FileID" );
		$hSQL->Select( "_PHP_CodeDoc_Files", $arrKeys,
					   "ParentDirID=?", array( $iParentDirID ),
					   "Name" );

		$arrFiles = array();
		$arrFileRow = $hSQL->FetchRow();
		while( $arrFileRow != NULL ) {
			if ( $bGetInfos )
				array_push( $arrFiles, $arrFileRow );
			else
				array_push( $arrFiles, $arrFileRow["FileID"] );
			$arrFileRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();
		
		return $arrFiles;
	}
	public function GetFileInfos( $iFileID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Files", NULL,
					   "FileID=?", array( $iFileID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrFileRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrFileRow;
	}
	public function GetFileID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Files", array( "FileID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrFileRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrFileRow["FileID"];
	}

	// Defines operations
	public function CreateDefine( $iParentFileID,
								  $strName, $strFileName, $iLineNumber,
								  $iBodyXLinkID,
								  $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = $strName;

		$hSQL->Insert( "_PHP_CodeDoc_Defines", array( "ParentFileID" 	 => $iParentFileID,
			                                          "ScopedNameID"     => $strScopedNameID,
													  "Name"			 => $strName,
													  "FileName"		 => $strFileName,
													  "LineNumber"		 => $iLineNumber,
													  "BodyLink"		 => $iBodyXLinkID,
													  "ShortDesc"		 => $strShortDesc,
													  "DocContent"		 => $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateDefine( $iDefineID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Defines", array( "ShortDesc"  => $strShortDesc,
													  "DocContent" => $strDocContent ),
					   "DefineID=?", array( $iDefineID ) );
	}
	public function ListDefines( $iParentFileID, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "DefineID" );
		$hSQL->Select( "_PHP_CodeDoc_Defines", $arrKeys,
					   "ParentFileID=?", array( $iParentFileID ),
					   "DefineID" );

		$arrDefines = array();
		$arrDefineRow = $hSQL->FetchRow();
		while( $arrDefineRow != NULL ) {
			if ( $bGetInfos )
				array_push( $arrDefines, $arrDefineRow );
			else
				array_push( $arrDefines, $arrDefineRow["DefineID"] );
			$arrDefineRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrDefines;
	}
	public function GetDefineInfos( $iDefineID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Defines", NULL,
					   "DefineID=?", array( $iDefineID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrDefineRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrDefineRow;
	}
	public function GetDefineID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Defines", array( "DefineID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		//assert( '$hSQL->GetRowCount() == 1' );
		$arrDefineRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrDefineRow["DefineID"];
	}

	// Macros operations
	public function CreateMacro( $iParentFileID,
								 $strName, $strFileName, $iLineNumber,
								 $arrParams, $iBodyXLinkID,
								 $strShortDesc, $strDocContent )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = $strName;

		$strParams = $hSystem->StringEncodeArray( $arrParams, ",", "#" );

		$hSQL->Insert( "_PHP_CodeDoc_Macros", array( "ParentFileID" 	=> $iParentFileID,
													 "ScopedNameID"     => $strScopedNameID,
													 "Name"			 	=> $strName,
													 "FileName"		 	=> $strFileName,
													 "LineNumber"		=> $iLineNumber,
													 "Params"		 	=> $strParams,
													 "BodyLink"		 	=> $iBodyXLinkID,
													 "ShortDesc"		=> $strShortDesc,
													 "DocContent"		=> $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateMacro( $iMacroID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Macros", array( "ShortDesc"  => $strShortDesc,
													 "DocContent" => $strDocContent ),
					   "MacroID=?", array( $iMacroID ) );
	}
	public function ListMacros( $iParentFileID, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "MacroID" );
		$hSQL->Select( "_PHP_CodeDoc_Macros", $arrKeys,
					   "ParentFileID=?", array( $iParentFileID ),
					   "MacroID" );

		$arrMacros = array();
		$arrMacroRow = $hSQL->FetchRow();
		while( $arrMacroRow != NULL ) {
			if ( $bGetInfos ) {
				$arrMacroRow["Params"] = $hSystem->StringDecodeArray( $arrMacroRow["Params"], ",", "#" );
				array_push( $arrMacros, $arrMacroRow );
			} else
				array_push( $arrMacros, $arrMacroRow["MacroID"] );
			$arrMacroRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrMacros;
	}
	public function GetMacroInfos( $iMacroID )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Macros", NULL,
					   "MacroID=?", array( $iMacroID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrMacroRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrMacroRow["Params"] = $hSystem->StringDecodeArray( $arrMacroRow["Params"], ",", "#" );
		return $arrMacroRow;
	}
	public function GetMacroID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Macros", array( "MacroID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		//assert( '$hSQL->GetRowCount() == 1' );
		$arrMacroRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrMacroRow["MacroID"];
	}

	// Namespaces operations
	public function CreateNamespace( $iParentFileID, $iParentNamespaceID,
									 $strName, $strFileName, $iLineNumber,
									 $strProtection,
									 $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = "";
		if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
			$arrNamespaceInfos = $this->GetNamespaceInfos( $iParentNamespaceID );
			$strScopedNameID = $arrNamespaceInfos["ScopedNameID"];
		}
		$strScopedNameID .= "::$strName";

		$hSQL->Insert( "_PHP_CodeDoc_Namespaces", array( "ParentFileID"		 => $iParentFileID,
													 	 "ParentNamespaceID" => $iParentNamespaceID,
													 	 "ScopedNameID"      => $strScopedNameID,
													 	 "Name"			 	 => $strName,
													 	 "FileName"		 	 => $strFileName,
													 	 "LineNumber"		 => $iLineNumber,
														 "Protection"		 => $strProtection,
													 	 "ShortDesc"		 => $strShortDesc,
													 	 "DocContent"		 => $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateNamespace( $iNamespaceID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Namespaces", array( "ShortDesc"  => $strShortDesc,
													 	 "DocContent" => $strDocContent ),
					   "NamespaceID=?", array( $iNamespaceID ) );
	}
	public function ListGlobalNamespaces( $iParentFileID, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$iNullID = CODEDOC_ID_NULL;

		$arrKeys = $bGetInfos ? NULL : array( "NamespaceID" );
		$hSQL->Select( "_PHP_CodeDoc_Namespaces", $arrKeys,
					   "ParentFileID=? AND ParentNamespaceID=$iNullID", array( $iParentFileID ),
					   "NamespaceID" );

		$arrNamespaces = array();
		$arrNamespaceRow = $hSQL->FetchRow();
		while( $arrNamespaceRow != NULL ) {
			if ( $bGetInfos )
				array_push( $arrNamespaces, $arrNamespaceRow );
			else
				array_push( $arrNamespaces, $arrNamespaceRow["NamespaceID"] );
			$arrNamespaceRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrNamespaces;
	}
	public function ListNestedNamespaces( $iParentNamespaceID, $strProtection, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "NamespaceID" );
		$hSQL->Select( "_PHP_CodeDoc_Namespaces", $arrKeys,
					   "ParentNamespaceID=? AND Protection='?'", array( $iParentNamespaceID, $strProtection ),
					   "NamespaceID" );

		$arrNamespaces = array();
		$arrNamespaceRow = $hSQL->FetchRow();
		while( $arrNamespaceRow != NULL ) {
			if ( $bGetInfos )
				array_push( $arrNamespaces, $arrNamespaceRow );
			else
				array_push( $arrNamespaces, $arrNamespaceRow["NamespaceID"] );
			$arrNamespaceRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrNamespaces;
	}
	public function GetNamespaceInfos( $iNamespaceID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Namespaces", NULL,
					   "NamespaceID=?", array( $iNamespaceID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrNamespaceRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrNamespaceRow;
	}
	public function GetNamespaceID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Namespaces", array( "NamespaceID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrNamespaceRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrNamespaceRow["NamespaceID"];
	}

	// Types operations
	public function CreateType( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
								$strName, $strFileName, $iLineNumber,
								$arrBaseTypes, $arrTemplateParams, $strProtection, $strMetaType,
								$strShortDesc, $strDocContent )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$strScopedNameID = "";
		if ( $iParentTypeID != CODEDOC_ID_NULL ) {
			$arrTypeInfos = $this->GetTypeInfos( $iParentTypeID );
			$strScopedNameID = $arrTypeInfos["ScopedNameID"];
		} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
			$arrNamespaceInfos = $this->GetNamespaceInfos( $iParentNamespaceID );
			$strScopedNameID = $arrNamespaceInfos["ScopedNameID"];
		}
		$strScopedNameID .= "::$strName";

		$strBaseTypes = $hSystem->StringEncodeMap( $arrBaseTypes, ",", "#" );
		$strTemplateParams = $hSystem->StringEncodeArray( $arrTemplateParams, ",", "#" );

		$hSQL->Insert( "_PHP_CodeDoc_Types", array( "ParentFileID"		=> $iParentFileID,
													"ParentNamespaceID" => $iParentNamespaceID,
													"ParentTypeID" 		=> $iParentTypeID,
													"ScopedNameID"      => $strScopedNameID,
													"Name"			 	=> $strName,
													"FileName"		 	=> $strFileName,
													"LineNumber"		=> $iLineNumber,
													"BaseTypes"			=> $strBaseTypes,
													"TemplateParams"	=> $strTemplateParams,
													"Protection"		=> $strProtection,
													"MetaType"			=> $strMetaType,
													"ShortDesc"		 	=> $strShortDesc,
													"DocContent"		=> $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function ResolveBaseTypes( $iTypeID, $arrBaseTypes )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$strBaseTypes = $hSystem->StringEncodeMap( $arrBaseTypes, ",", "#" );

		$hSQL->Update( "_PHP_CodeDoc_Types", array( "BaseTypes"  => $strBaseTypes ),
					   "TypeID=?", array( $iTypeID ) );
	}
	public function UpdateType( $iTypeID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Types", array( "ShortDesc"  => $strShortDesc,
													"DocContent" => $strDocContent ),
					   "TypeID=?", array( $iTypeID ) );
	}
	public function ListGlobalTypes( $iParentFileID, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$iNullID = CODEDOC_ID_NULL;

		$arrKeys = $bGetInfos ? NULL : array( "TypeID" );
		$hSQL->Select( "_PHP_CodeDoc_Types", $arrKeys,
					   "ParentFileID=? AND ParentNamespaceID=$iNullID AND ParentTypeID=$iNullID", array( $iParentFileID ),
					   "TypeID" );

		$arrTypes = array();
		$arrTypeRow = $hSQL->FetchRow();
		while( $arrTypeRow != NULL ) {
			if ( $bGetInfos ) {
				$arrTypeRow["BaseTypes"] = $hSystem->StringDecodeMap( $arrTypeRow["BaseTypes"], ",", "#" );
				$arrTypeRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrTypeRow["TemplateParams"], ",", "#" );
				array_push( $arrTypes, $arrTypeRow );
			} else
				array_push( $arrTypes, $arrTypeRow["TypeID"] );
			$arrTypeRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrTypes;
	}
	public function ListNamespaceTypes( $iParentNamespaceID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "TypeID" );
		$hSQL->Select( "_PHP_CodeDoc_Types", $arrKeys,
					   "ParentNamespaceID=? AND Protection='?'", array( $iParentNamespaceID, $strProtection ),
					   "TypeID" );

		$arrTypes = array();
		$arrTypeRow = $hSQL->FetchRow();
		while( $arrTypeRow != NULL ) {
			if ( $bGetInfos ) {
				$arrTypeRow["BaseTypes"] = $hSystem->StringDecodeMap( $arrTypeRow["BaseTypes"], ",", "#" );
				$arrTypeRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrTypeRow["TemplateParams"], ",", "#" );
				array_push( $arrTypes, $arrTypeRow );
			} else
				array_push( $arrTypes, $arrTypeRow["TypeID"] );
			$arrTypeRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrTypes;
	}
	public function ListNestedTypes( $iParentTypeID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "TypeID" );
		$hSQL->Select( "_PHP_CodeDoc_Types", $arrKeys,
					   "ParentTypeID=? AND Protection='?'", array( $iParentTypeID, $strProtection ),
					   "TypeID" );

		$arrTypes = array();
		$arrTypeRow = $hSQL->FetchRow();
		while( $arrTypeRow != NULL ) {
			if ( $bGetInfos ) {
				$arrTypeRow["BaseTypes"] = $hSystem->StringDecodeMap( $arrTypeRow["BaseTypes"], ",", "#" );
				$arrTypeRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrTypeRow["TemplateParams"], ",", "#" );
				array_push( $arrTypes, $arrTypeRow );
			} else
				array_push( $arrTypes, $arrTypeRow["TypeID"] );
			$arrTypeRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrTypes;
	}
	public function ListDerivedTypes( $iTypeID, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "TypeID" );
		$hSQL->Select( "_PHP_CodeDoc_Types", $arrKeys,
					   "BaseTypes LIKE '?#%' OR BaseTypes LIKE '%,?#%'", array( "LARG", $iTypeID, "LARG", $iTypeID ),
					   "TypeID" );

		$arrTypes = array();
		$arrTypeRow = $hSQL->FetchRow();
		while( $arrTypeRow != NULL ) {
			if ( $bGetInfos ) {
				$arrTypeRow["BaseTypes"] = $hSystem->StringDecodeMap( $arrTypeRow["BaseTypes"], ",", "#" );
				$arrTypeRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrTypeRow["TemplateParams"], ",", "#" );
				array_push( $arrTypes, $arrTypeRow );
			} else
				array_push( $arrTypes, $arrTypeRow["TypeID"] );
			$arrTypeRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrTypes;
	}
	public function GetTypeInfos( $iTypeID )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Types", NULL,
					   "TypeID=?", array( $iTypeID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrTypeRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrTypeRow["BaseTypes"] = $hSystem->StringDecodeMap( $arrTypeRow["BaseTypes"], ",", "#" );
		$arrTypeRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrTypeRow["TemplateParams"], ",", "#" );
		return $arrTypeRow;
	}
	public function GetTypeID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Types", array( "TypeID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrTypeRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrTypeRow["TypeID"];
	}

	// Enums operations
	public function CreateEnum( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
								$strName, $strFileName, $iLineNumber,
								$strProtection, $arrEnumValues,
								$strShortDesc, $strDocContent )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = "";
		if ( $iParentTypeID != CODEDOC_ID_NULL ) {
			$arrTypeInfos = $this->GetTypeInfos( $iParentTypeID );
			$strScopedNameID = $arrTypeInfos["ScopedNameID"];
		} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
			$arrNamespaceInfos = $this->GetNamespaceInfos( $iParentNamespaceID );
			$strScopedNameID = $arrNamespaceInfos["ScopedNameID"];
		}
		$strScopedNameID .= "::$strName";

		$strEnumValues = $hSystem->StringEncodeMap( $arrEnumValues, ",", "#" );

		$hSQL->Insert( "_PHP_CodeDoc_Enums", array( "ParentFileID"		=> $iParentFileID,
													"ParentNamespaceID" => $iParentNamespaceID,
													"ParentTypeID" 		=> $iParentTypeID,
													"ScopedNameID" 		=> $strScopedNameID,
													"Name"			 	=> $strName,
													"FileName"		 	=> $strFileName,
													"LineNumber"		=> $iLineNumber,
													"Protection"		=> $strProtection,
													"EnumValues"		=> $strEnumValues,
													"ShortDesc"		 	=> $strShortDesc,
													"DocContent"		=> $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateEnum( $iEnumID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Enums", array( "ShortDesc"  => $strShortDesc,
													"DocContent" => $strDocContent ),
					   "EnumID=?", array( $iEnumID ) );
	}
	public function ListGlobalEnums( $iParentFileID, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$iNullID = CODEDOC_ID_NULL;

		$arrKeys = $bGetInfos ? NULL : array( "EnumID" );
		$hSQL->Select( "_PHP_CodeDoc_Enums", $arrKeys,
					   "ParentFileID=? AND ParentNamespaceID=$iNullID AND ParentTypeID=$iNullID", array( $iParentFileID ),
					   "EnumID" );

		$arrEnums = array();
		$arrEnumRow = $hSQL->FetchRow();
		while( $arrEnumRow != NULL ) {
			if ( $bGetInfos ) {
				$arrEnumRow["EnumValues"] = $hSystem->StringDecodeMap( $arrEnumRow["EnumValues"], ",", "#" );
				array_push( $arrEnums, $arrEnumRow );
			} else
				array_push( $arrEnums, $arrEnumRow["EnumID"] );
			$arrEnumRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrEnums;
	}
	public function ListNamespaceEnums( $iParentNamespaceID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "EnumID" );
		$hSQL->Select( "_PHP_CodeDoc_Enums", $arrKeys,
					   "ParentNamespaceID=? AND Protection='?'", array( $iParentNamespaceID, $strProtection ),
					   "EnumID" );

		$arrEnums = array();
		$arrEnumRow = $hSQL->FetchRow();
		while( $arrEnumRow != NULL ) {
			if ( $bGetInfos ) {
				$arrEnumRow["EnumValues"] = $hSystem->StringDecodeMap( $arrEnumRow["EnumValues"], ",", "#" );
				array_push( $arrEnums, $arrEnumRow );
			} else
				array_push( $arrEnums, $arrEnumRow["EnumID"] );
			$arrEnumRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrEnums;
	}
	public function ListNestedEnums( $iParentTypeID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "EnumID" );
		$hSQL->Select( "_PHP_CodeDoc_Enums", $arrKeys,
					   "ParentTypeID=? AND Protection='?'", array( $iParentTypeID, $strProtection ),
					   "EnumID" );

		$arrEnums = array();
		$arrEnumRow = $hSQL->FetchRow();
		while( $arrEnumRow != NULL ) {
			if ( $bGetInfos ) {
				$arrEnumRow["EnumValues"] = $hSystem->StringDecodeMap( $arrEnumRow["EnumValues"], ",", "#" );
				array_push( $arrEnums, $arrEnumRow );
			} else
				array_push( $arrEnums, $arrEnumRow["EnumID"] );
			$arrEnumRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrEnums;
	}
	public function GetEnumInfos( $iEnumID )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Enums", NULL,
					   "EnumID=?", array( $iEnumID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrEnumRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrEnumRow["EnumValues"] = $hSystem->StringDecodeMap( $arrEnumRow["EnumValues"], ",", "#" );
		return $arrEnumRow;
	}
	public function GetEnumID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Enums", array( "EnumID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		//assert( '$hSQL->GetRowCount() == 1' );
		$arrEnumRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrEnumRow["EnumID"];
	}

	// Typedefs operations
	public function CreateTypedef( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
								   $strName, $strFileName, $iLineNumber,
								   $strProtection, $iTargetTypeXLinkID, $arrParams,
								   $strShortDesc, $strDocContent )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = "";
		if ( $iParentTypeID != CODEDOC_ID_NULL ) {
			$arrTypeInfos = $this->GetTypeInfos( $iParentTypeID );
			$strScopedNameID = $arrTypeInfos["ScopedNameID"];
		} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
			$arrNamespaceInfos = $this->GetNamespaceInfos( $iParentNamespaceID );
			$strScopedNameID = $arrNamespaceInfos["ScopedNameID"];
		}
		$strScopedNameID .= "::$strName";

		$strParams = $hSystem->StringEncodeArray( $arrParams, ",", "#" );

		$hSQL->Insert( "_PHP_CodeDoc_Typedefs", array( "ParentFileID"	   => $iParentFileID,
													   "ParentNamespaceID" => $iParentNamespaceID,
													   "ParentTypeID" 	   => $iParentTypeID,
													   "ScopedNameID" 	   => $strScopedNameID,
													   "Name"			   => $strName,
													   "FileName"		   => $strFileName,
													   "LineNumber"		   => $iLineNumber,
													   "Protection"		   => $strProtection,
													   "TargetTypeLink"	   => $iTargetTypeXLinkID,
													   "Params"			   => $strParams,
													   "ShortDesc"		   => $strShortDesc,
													   "DocContent"		   => $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateTypedef( $iTypedefID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Typedefs", array( "ShortDesc"  => $strShortDesc,
													   "DocContent" => $strDocContent ),
					   "TypedefID=?", array( $iTypedefID ) );
	}
	public function ListGlobalTypedefs( $iParentFileID, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$iNullID = CODEDOC_ID_NULL;

		$arrKeys = $bGetInfos ? NULL : array( "TypedefID" );
		$hSQL->Select( "_PHP_CodeDoc_Typedefs", $arrKeys,
					   "ParentFileID=? AND ParentNamespaceID=$iNullID AND ParentTypeID=$iNullID", array( $iParentFileID ),
					   "TypedefID" );

		$arrTypedefs = array();
		$arrTypedefRow = $hSQL->FetchRow();
		while( $arrTypedefRow != NULL ) {
			if ( $bGetInfos ) {
				$arrTypedefRow["Params"] = $hSystem->StringDecodeArray( $arrTypedefRow["Params"], ",", "#" );
				array_push( $arrTypedefs, $arrTypedefRow );
			} else
				array_push( $arrTypedefs, $arrTypedefRow["TypedefID"] );
			$arrTypedefRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrTypedefs;
	}
	public function ListNamespaceTypedefs( $iParentNamespaceID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "TypedefID" );
		$hSQL->Select( "_PHP_CodeDoc_Typedefs", $arrKeys,
					   "ParentNamespaceID=? AND Protection='?'", array( $iParentNamespaceID, $strProtection ),
					   "TypedefID" );

		$arrTypedefs = array();
		$arrTypedefRow = $hSQL->FetchRow();
		while( $arrTypedefRow != NULL ) {
			if ( $bGetInfos ) {
				$arrTypedefRow["Params"] = $hSystem->StringDecodeArray( $arrTypedefRow["Params"], ",", "#" );
				array_push( $arrTypedefs, $arrTypedefRow );
			} else
				array_push( $arrTypedefs, $arrTypedefRow["TypedefID"] );
			$arrTypedefRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrTypedefs;
	}
	public function ListNestedTypedefs( $iParentTypeID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "TypedefID" );
		$hSQL->Select( "_PHP_CodeDoc_Typedefs", $arrKeys,
					   "ParentTypeID=? AND Protection='?'", array( $iParentTypeID, $strProtection ),
					   "TypedefID" );

		$arrTypedefs = array();
		$arrTypedefRow = $hSQL->FetchRow();
		while( $arrTypedefRow != NULL ) {
			if ( $bGetInfos ) {
				$arrTypedefRow["Params"] = $hSystem->StringDecodeArray( $arrTypedefRow["Params"], ",", "#" );
				array_push( $arrTypedefs, $arrTypedefRow );
			} else
				array_push( $arrTypedefs, $arrTypedefRow["TypedefID"] );
			$arrTypedefRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrTypedefs;
	}
	public function GetTypedefInfos( $iTypedefID )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Typedefs", NULL,
					   "TypedefID=?", array( $iTypedefID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrTypedefRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrTypedefRow["Params"] = $hSystem->StringDecodeArray( $arrTypedefRow["Params"], ",", "#" );
		return $arrTypedefRow;
	}
	public function GetTypedefID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Typedefs", array( "TypedefID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		//assert( '$hSQL->GetRowCount() == 1' );
		$arrTypedefRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrTypedefRow["TypedefID"];
	}

	// Variables operations
	public function CreateVariable( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
									$strName, $strFileName, $iLineNumber,
									$arrTemplateParams, $strProtection, $bStatic, $bConst, $bVolatile, $bMutable,
									$iInstanceTypeXLinkID, $iInitializeXLinkID,
									$strShortDesc, $strDocContent )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = "";
		if ( $iParentTypeID != CODEDOC_ID_NULL ) {
			$arrTypeInfos = $this->GetTypeInfos( $iParentTypeID );
			$strScopedNameID = $arrTypeInfos["ScopedNameID"];
		} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
			$arrNamespaceInfos = $this->GetNamespaceInfos( $iParentNamespaceID );
			$strScopedNameID = $arrNamespaceInfos["ScopedNameID"];
		}
		$strScopedNameID .= "::$strName";

		$strTemplateParams = $hSystem->StringEncodeArray( $arrTemplateParams, ",", "#" );

		$hSQL->Insert( "_PHP_CodeDoc_Variables", array( "ParentFileID"	   	=> $iParentFileID,
													   	"ParentNamespaceID"	=> $iParentNamespaceID,
													   	"ParentTypeID" 	   	=> $iParentTypeID,
														"ScopedNameID" 	    => $strScopedNameID,
													   	"Name"			   	=> $strName,
													   	"FileName"		   	=> $strFileName,
													   	"LineNumber"		=> $iLineNumber,
													   	"TemplateParams"	=> $strTemplateParams,
													   	"Protection"		=> $strProtection,
													   	"Static"			=> $bStatic ? "1" : "0",
													   	"Const"				=> $bConst ? "1" : "0",
													   	"Volatile"			=> $bVolatile ? "1" : "0",
													   	"Mutable"			=> $bMutable ? "1" : "0",
													   	"InstanceTypeLink"	=> $iInstanceTypeXLinkID,
													   	"InitializeLink"	=> $iInitializeXLinkID,
													   	"ShortDesc"		   	=> $strShortDesc,
													   	"DocContent"		=> $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateVariable( $iVariableID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Variables", array( "ShortDesc"  => $strShortDesc,
													   	"DocContent" => $strDocContent ),
					   "VariableID=?", array( $iVariableID ) );
	}
	public function ListGlobalVariables( $iParentFileID, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$iNullID = CODEDOC_ID_NULL;

		$arrKeys = $bGetInfos ? NULL : array( "VariableID" );
		$hSQL->Select( "_PHP_CodeDoc_Variables", $arrKeys,
					   "ParentFileID=? AND ParentNamespaceID=$iNullID AND ParentTypeID=$iNullID", array( $iParentFileID ),
					   "VariableID" );

		$arrVariables = array();
		$arrVariableRow = $hSQL->FetchRow();
		while( $arrVariableRow != NULL ) {
			if ( $bGetInfos ) {
				$arrVariableRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrVariableRow["TemplateParams"], ",", "#" );
				$arrVariableRow["Static"] = ( $arrVariableRow["Static"] == "1" );
				$arrVariableRow["Const"] = ( $arrVariableRow["Const"] == "1" );
				$arrVariableRow["Volatile"] = ( $arrVariableRow["Volatile"] == "1" );
				$arrVariableRow["Mutable"] = ( $arrVariableRow["Mutable"] == "1" );
				array_push( $arrVariables, $arrVariableRow );
			} else
				array_push( $arrVariables, $arrVariableRow["VariableID"] );
			$arrVariableRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrVariables;
	}
	public function ListNamespaceVariables( $iParentNamespaceID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "VariableID" );
		$hSQL->Select( "_PHP_CodeDoc_Variables", $arrKeys,
					   "ParentNamespaceID=? AND Protection='?'", array( $iParentNamespaceID, $strProtection ),
					   "VariableID" );

		$arrVariables = array();
		$arrVariableRow = $hSQL->FetchRow();
		while( $arrVariableRow != NULL ) {
			if ( $bGetInfos ) {
				$arrVariableRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrVariableRow["TemplateParams"], ",", "#" );
				$arrVariableRow["Static"] = ( $arrVariableRow["Static"] == "1" );
				$arrVariableRow["Const"] = ( $arrVariableRow["Const"] == "1" );
				$arrVariableRow["Volatile"] = ( $arrVariableRow["Volatile"] == "1" );
				$arrVariableRow["Mutable"] = ( $arrVariableRow["Mutable"] == "1" );
				array_push( $arrVariables, $arrVariableRow );
			} else
				array_push( $arrVariables, $arrVariableRow["VariableID"] );
			$arrVariableRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrVariables;
	}
	public function ListNestedVariables( $iParentTypeID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "VariableID" );
		$hSQL->Select( "_PHP_CodeDoc_Variables", $arrKeys,
					   "ParentTypeID=? AND Protection='?'", array( $iParentTypeID, $strProtection ),
					   "VariableID" );

		$arrVariables = array();
		$arrVariableRow = $hSQL->FetchRow();
		while( $arrVariableRow != NULL ) {
			if ( $bGetInfos ) {
				$arrVariableRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrVariableRow["TemplateParams"], ",", "#" );
				$arrVariableRow["Static"] = ( $arrVariableRow["Static"] == "1" );
				$arrVariableRow["Const"] = ( $arrVariableRow["Const"] == "1" );
				$arrVariableRow["Volatile"] = ( $arrVariableRow["Volatile"] == "1" );
				$arrVariableRow["Mutable"] = ( $arrVariableRow["Mutable"] == "1" );
				array_push( $arrVariables, $arrVariableRow );
			} else
				array_push( $arrVariables, $arrVariableRow["VariableID"] );
			$arrVariableRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrVariables;
	}
	public function GetVariableInfos( $iVariableID )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Variables", NULL,
					   "VariableID=?", array( $iVariableID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrVariableRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrVariableRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrVariableRow["TemplateParams"], ",", "#" );
		$arrVariableRow["Static"] = ( $arrVariableRow["Static"] == "1" );
		$arrVariableRow["Const"] = ( $arrVariableRow["Const"] == "1" );
		$arrVariableRow["Volatile"] = ( $arrVariableRow["Volatile"] == "1" );
		$arrVariableRow["Mutable"] = ( $arrVariableRow["Mutable"] == "1" );
		return $arrVariableRow;
	}
	public function GetVariableID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Variables", array( "VariableID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrVariableRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrVariableRow["VariableID"];
	}

	// Functions operations
	public function CreateFunction( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
									$strName, $strFileName, $iLineNumber,
									$arrTemplateParams, $strProtection, $bInline, $bStatic, $strVirtual, $bConst,
									$iReturnTypeXLinkID, $arrParams,
									$strShortDesc, $strDocContent )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();
		
		$strScopedNameID = "";
		if ( $iParentTypeID != CODEDOC_ID_NULL ) {
			$arrTypeInfos = $this->GetTypeInfos( $iParentTypeID );
			$strScopedNameID = $arrTypeInfos["ScopedNameID"];
		} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
			$arrNamespaceInfos = $this->GetNamespaceInfos( $iParentNamespaceID );
			$strScopedNameID = $arrNamespaceInfos["ScopedNameID"];
		}
		$strScopedNameID .= "::$strName(";
		$iParamCount = count( $arrParams );
		for( $i = 0; $i < $iParamCount; ++$i ) {
			$iTypeXLinkID = $arrParams[$i][1];
			$arrXLinkInfos = $this->GetXLinkInfos( $iTypeXLinkID );
			$strScopedNameID .= ( "::" . $arrXLinkInfos["LinkText"] );
//			$arrXReferences = $this->ListXReferences( $iTypeXLinkID, true );
//			$iXRefCount = count( $arrXReferences );
//			if ( $iXRefCount == 0 ) // Built-in type
//				$strScopedNameID .= ( "::" . $arrXLinkInfos["LinkText"] );
//			else if ( $iXRefCount != 1 ) // Inline-defined function pointer ...
//				$strScopedNameID .= "::<callback_$i>";
//			else { // Declared type
//				$arrXRefInfos = $arrXReferences[0];
//				$strTargetKind = $arrXRefInfos["TargetKind"];
//				$iTargetID = $arrXRefInfos["TargetID"];
//				$arrTargetInfos = NULL;
//				switch( $strTargetKind ) {
//					case CODEDOC_XREFKIND_DEFINE:   $arrTargetInfos = $this->GetDefineInfos( $iTargetID ); break;
//					case CODEDOC_XREFKIND_MACRO:    $arrTargetInfos = $this->GetMacroInfos( $iTargetID ); break;
//					case CODEDOC_XREFKIND_TYPE:     $arrTargetInfos = $this->GetTypeInfos( $iTargetID ); break;
//					case CODEDOC_XREFKIND_ENUM:     $arrTargetInfos = $this->GetEnumInfos( $iTargetID ); break;
//					case CODEDOC_XREFKIND_TYPEDEF:  $arrTargetInfos = $this->GetTypedefInfos( $iTargetID ); break;
//					case CODEDOC_XREFKIND_VARIABLE: $arrTargetInfos = $this->GetVariableInfos( $iTargetID ); break;
//					case CODEDOC_XREFKIND_FUNCTION: $arrTargetInfos = $this->GetFunctionInfos( $iTargetID ); break;
//					default: assert( 'false' ); break;
//				}
//				$strScopedNameID .= ( "::" . $arrTargetInfos["ScopedNameID"] );
//			}
			if ( $i < ($iParamCount - 1) )
				$strScopedNameID .= ",";
		}
		$strScopedNameID .= ")";
		if ( $bConst )
			$strScopedNameID .= "const";
		
		$hSQL->Select( "_PHP_CodeDoc_Functions", NULL,
					   "ScopedNameID='?'", array( $strScopedNameID ) );
		if ( $hSQL->GetRowCount() != 0 ) {
			assert( '$hSQL->GetRowCount() == 1' );
			$arrFunctionRow = $hSQL->FetchRow();		
			$hSQL->FreeResults();
			assert( $arrFunctionRow["ParentTypeID"] == $iParentTypeID );
			print_r( "<br><br> Duplicate function : " . $strScopedNameID );
			return $arrFunctionRow["FunctionID"];
		}

		$strTemplateParams = $hSystem->StringEncodeArray( $arrTemplateParams, ",", "#" );
		$strParams = $hSystem->StringEncodeArray( $arrParams, ",", "#" );

		$hSQL->Insert( "_PHP_CodeDoc_Functions", array( "ParentFileID"	   	=> $iParentFileID,
													   	"ParentNamespaceID"	=> $iParentNamespaceID,
													   	"ParentTypeID" 	   	=> $iParentTypeID,
														"ScopedNameID" 	    => $strScopedNameID,
													   	"Name"			   	=> $strName,
													   	"FileName"		   	=> $strFileName,
													   	"LineNumber"		=> $iLineNumber,
													   	"TemplateParams"	=> $strTemplateParams,
													   	"Protection"		=> $strProtection,
													   	"Inline"			=> $bInline ? "1" : "0",
													   	"Static"			=> $bStatic ? "1" : "0",
													   	"Virtual"			=> $strVirtual,
													   	"Const"				=> $bConst ? "1" : "0",
													   	"ReturnTypeLink"	=> $iReturnTypeXLinkID,
													   	"Params"			=> $strParams,
													   	"ShortDesc"		   	=> $strShortDesc,
													   	"DocContent"		=> $strDocContent ) );
		return $hSQL->GetAutoIncrement();
	}
	public function UpdateFunction( $iFunctionID, $strShortDesc, $strDocContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_Functions", array( "ShortDesc"  => $strShortDesc,
													   	"DocContent" => $strDocContent ),
					   "FunctionID=?", array( $iFunctionID ) );
	}
	public function ListGlobalFunctions( $iParentFileID, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$iNullID = CODEDOC_ID_NULL;

		$arrKeys = $bGetInfos ? NULL : array( "FunctionID" );
		$hSQL->Select( "_PHP_CodeDoc_Functions", $arrKeys,
					   "ParentFileID=? AND ParentNamespaceID=$iNullID AND ParentTypeID=$iNullID", array( $iParentFileID ),
					   "FunctionID" );

		$arrFunctions = array();
		$arrFunctionRow = $hSQL->FetchRow();
		while( $arrFunctionRow != NULL ) {
			if ( $bGetInfos ) {
				$arrFunctionRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrFunctionRow["TemplateParams"], ",", "#" );
				$arrFunctionRow["Inline"] = ( $arrFunctionRow["Inline"] == "1" );
				$arrFunctionRow["Static"] = ( $arrFunctionRow["Static"] == "1" );
				$arrFunctionRow["Const"] = ( $arrFunctionRow["Const"] == "1" );
				$arrFunctionRow["Params"] = $hSystem->StringDecodeArray( $arrFunctionRow["Params"], ",", "#" );
				array_push( $arrFunctions, $arrFunctionRow );
			} else
				array_push( $arrFunctions, $arrFunctionRow["FunctionID"] );
			$arrFunctionRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrFunctions;
	}
	public function ListNamespaceFunctions( $iParentNamespaceID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "FunctionID" );
		$hSQL->Select( "_PHP_CodeDoc_Functions", $arrKeys,
					   "ParentNamespaceID=? AND Protection='?'", array( $iParentNamespaceID, $strProtection ),
					   "FunctionID" );

		$arrFunctions = array();
		$arrFunctionRow = $hSQL->FetchRow();
		while( $arrFunctionRow != NULL ) {
			if ( $bGetInfos ) {
				$arrFunctionRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrFunctionRow["TemplateParams"], ",", "#" );
				$arrFunctionRow["Inline"] = ( $arrFunctionRow["Inline"] == "1" );
				$arrFunctionRow["Static"] = ( $arrFunctionRow["Static"] == "1" );
				$arrFunctionRow["Const"] = ( $arrFunctionRow["Const"] == "1" );
				$arrFunctionRow["Params"] = $hSystem->StringDecodeArray( $arrFunctionRow["Params"], ",", "#" );
				array_push( $arrFunctions, $arrFunctionRow );
			} else
				array_push( $arrFunctions, $arrFunctionRow["FunctionID"] );
			$arrFunctionRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrFunctions;
	}
	public function ListNestedFunctions( $iParentTypeID, $strProtection, $bGetInfos = false )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "FunctionID" );
		$hSQL->Select( "_PHP_CodeDoc_Functions", $arrKeys,
					   "ParentTypeID=? AND Protection='?'", array( $iParentTypeID, $strProtection ),
					   "FunctionID" );

		$arrFunctions = array();
		$arrFunctionRow = $hSQL->FetchRow();
		while( $arrFunctionRow != NULL ) {
			if ( $bGetInfos ) {
				$arrFunctionRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrFunctionRow["TemplateParams"], ",", "#" );
				$arrFunctionRow["Inline"] = ( $arrFunctionRow["Inline"] == "1" );
				$arrFunctionRow["Static"] = ( $arrFunctionRow["Static"] == "1" );
				$arrFunctionRow["Const"] = ( $arrFunctionRow["Const"] == "1" );
				$arrFunctionRow["Params"] = $hSystem->StringDecodeArray( $arrFunctionRow["Params"], ",", "#" );
				array_push( $arrFunctions, $arrFunctionRow );
			} else
				array_push( $arrFunctions, $arrFunctionRow["FunctionID"] );
			$arrFunctionRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrFunctions;
	}
	public function GetFunctionInfos( $iFunctionID )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Functions", NULL,
					   "FunctionID=?", array( $iFunctionID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrFunctionRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrFunctionRow["TemplateParams"] = $hSystem->StringDecodeArray( $arrFunctionRow["TemplateParams"], ",", "#" );
		$arrFunctionRow["Inline"] = ( $arrFunctionRow["Inline"] == "1" );
		$arrFunctionRow["Static"] = ( $arrFunctionRow["Static"] == "1" );
		$arrFunctionRow["Const"] = ( $arrFunctionRow["Const"] == "1" );
		$arrFunctionRow["Params"] = $hSystem->StringDecodeArray( $arrFunctionRow["Params"], ",", "#" );
		return $arrFunctionRow;
	}
	public function GetFunctionID( $strScopedNameID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_Functions", array( "FunctionID" ),
					   "ScopedNameID='?'", array( $strScopedNameID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrFunctionRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrFunctionRow["FunctionID"];
	}

	// XLinks operations
	public function CreateXLink( $strLinkText )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Insert( "_PHP_CodeDoc_CrossedLinks", array( "LinkText" => $strLinkText ) );
		return $hSQL->GetAutoIncrement();
	}
	public function GetXLinkInfos( $iXLinkID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_CrossedLinks", NULL,
					   "XLinkID=?", array( $iXLinkID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrXLinkRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrXLinkRow;
	}

	// XReferences operations
	public function CreateXReference( $iParentXLinkID, $strTargetKind, $iTargetID, $strReferenceText )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Insert( "_PHP_CodeDoc_CrossedReferences", array( "ParentXLinkID" => $iParentXLinkID,
													   			"TargetKind"	=> $strTargetKind,
													   			"TargetID" 	   	=> $iTargetID,
													   			"ReferenceText" => $strReferenceText ) );
		return $hSQL->GetAutoIncrement();
	}
	public function ResolveXReference( $iXReferenceID, $strTargetKind, $iTargetID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_CodeDoc_CrossedReferences", array( "TargetKind"  => $strTargetKind,
													   			"TargetID" => $iTargetID ),
					   "XReferenceID=?", array( $iXReferenceID ) );
	}
	public function ListXReferences( $iParentXLinkID, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "XReferenceID" );
		$hSQL->Select( "_PHP_CodeDoc_CrossedReferences", $arrKeys,
					   "ParentXLinkID=?", array( $iParentXLinkID ),
					   "XReferenceID" );

		$arrXReferences = array();
		$arrXReferenceRow = $hSQL->FetchRow();
		while( $arrXReferenceRow != NULL ) {
			if ( $bGetInfos )
				array_push( $arrXReferences, $arrXReferenceRow );
			else
				array_push( $arrXReferences, $arrXReferenceRow["XReferenceID"] );
			$arrXReferenceRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrXReferences;
	}
	public function GetXReferenceInfos( $iXReferenceID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_CodeDoc_CrossedReferences", NULL,
					   "XReferenceID=?", array( $iXReferenceID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrXReferenceRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrXReferenceRow;
	}

	// Main routines
	public function InitializeTagRequests()
	{
		$hAJAX = AJAXManager::GetSingleton();

		$hAJAX->GenerateTagRequest( "CodeDocContent", "PHP/Modules/CodeDoc/_AJAX_CodeDocContent.php",
													  array( "CodeDocEvent", "CodeDocEventParam" ),
													  "UpdateMenuAfterContent(false);" );
		$hAJAX->GenerateTagRequest( "CodeDocHeader", "PHP/Modules/CodeDoc/_AJAX_CodeDocHeader.php",
													 array( "CodeDocEvent", "CodeDocEventParam" ),
													 "UpdateMenuAfterContent(false);" );
		$hAJAX->UpdateTag( false, "CodeDocContent", array( "'DisplayRoot'", "''" ) );
		$hAJAX->UpdateTag( false, "CodeDocHeader", array( "'DisplayRoot'", "''" ) );
	}

	public function Display()
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetID( "CodeDocHeader" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level0 ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayHeader( CODEDOC_DISPLAY_ROOT, "" );
		$hHTML->DivEnd();

		$hHTML->SetID( "CodeDocContent" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level0 ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayContent( CODEDOC_DISPLAY_ROOT, "" );
		$hHTML->DivEnd();
	}

	// Internal routines (don't call yourself !)
	public function _DisplayHeader( $strEvent, $strEventParam )
	{
		$hHTML = HTMLManager::GetSingleton();

		$hHTML->SetID( "CodeDocHeader_Commands" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1H ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayGenerator();
		$hHTML->DivEnd();

		$hHTML->SetID( "CodeDocHeader_NavigationTree" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1H ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayNavigationTree( $strEvent, $strEventParam );
		$hHTML->DivEnd();
	}
	public function _DisplayContent( $strEvent, $strEventParam )
	{
		// Update from event
		$this->_Update( $strEvent, $strEventParam );

		// Display response
		switch( $strEvent ) {
			case CODEDOC_DISPLAY_ROOT: 			 $this->_DisplayRoot(); 		     break;
			case CODEDOC_DISPLAY_GETTINGSTARTED: $this->_DisplayGettingStarted();    break;
			case CODEDOC_DISPLAY_CONVENTIONS:	 $this->_DisplayCodingConventions(); break;
			case CODEDOC_DISPLAY_FRAMEWORK:		 $this->_DisplayBasicFramework();	 break;
			case CODEDOC_DISPLAY_MANUAL:		 $this->_DisplayDevelopperManual();	 break;
			case CODEDOC_DISPLAY_REFERENCE: 	 $this->_DisplayReference(); 	     break;
			case CODEDOC_DISPLAY_DIRECTORY: 	 $this->_DisplayDirectory( intval($strEventParam) ); break;
			case CODEDOC_DISPLAY_FILE: 			 $this->_DisplayFile( intval($strEventParam) ); 	 break;
			case CODEDOC_DISPLAY_DEFINE: 		 $this->_DisplayDefine( intval($strEventParam) ); 	 break;
			case CODEDOC_DISPLAY_MACRO: 		 $this->_DisplayMacro( intval($strEventParam) ); 	 break;
			case CODEDOC_DISPLAY_NAMESPACE: 	 $this->_DisplayNamespace( intval($strEventParam) ); break;
			case CODEDOC_DISPLAY_TYPE: 			 $this->_DisplayType( intval($strEventParam) ); 	 break;
			case CODEDOC_DISPLAY_ENUM:		 	 $this->_DisplayEnum( intval($strEventParam) );		 break;
			case CODEDOC_DISPLAY_TYPEDEF:		 $this->_DisplayTypedef( intval($strEventParam) );	 break;
			case CODEDOC_DISPLAY_VARIABLE:		 $this->_DisplayVariable( intval($strEventParam) );	 break;
			case CODEDOC_DISPLAY_FUNCTION: 		 $this->_DisplayFunction( intval($strEventParam) );	 break;
			default: assert( 'false' ); break;
		}
	}

	// Helpers
	private function _GenerateTables()
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->ConnectTemporary();

		// Documentation table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Documentation ( ";
		$strQuery .= "Name char(32) NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( Name ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Directories table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Directories ( ";
		$strQuery .= "DirectoryID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentDirID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "Path tinytext NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( DirectoryID ), ";
		$strQuery .= "FOREIGN KEY ( ParentDirID ) REFERENCES _PHP_CodeDoc_Directories( DirectoryID ), ";
		$strQuery .= "UNIQUE ( ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Files table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Files ( ";
		$strQuery .= "FileID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentDirID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "Path tinytext NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( FileID ), ";
		$strQuery .= "FOREIGN KEY ( ParentDirID ) REFERENCES _PHP_CodeDoc_Directories( DirectoryID ), ";
		$strQuery .= "UNIQUE ( ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );
		
		// Defines table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Defines ( ";
		$strQuery .= "DefineID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "BodyLink int UNSIGNED NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( DefineID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "UNIQUE ( ParentFileID, ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );
	
		// Macros table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Macros ( ";
		$strQuery .= "MacroID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "Params tinytext NOT NULL, "; // "Name#XLinkID#XLinkID, ..."
		$strQuery .= "BodyLink int UNSIGNED NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( MacroID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "UNIQUE ( ParentFileID, ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Namespaces table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Namespaces ( ";
		$strQuery .= "NamespaceID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentNamespaceID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "Protection char(16) NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( NamespaceID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "FOREIGN KEY ( ParentNamespaceID ) REFERENCES _PHP_CodeDoc_Namespaces( NamespaceID ), ";
		$strQuery .= "UNIQUE ( ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Types table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Types ( ";
		$strQuery .= "TypeID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentNamespaceID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentTypeID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "BaseTypes tinytext NOT NULL, "; 	   // "TypeID#DerivMethod, ..."
		$strQuery .= "TemplateParams tinytext NOT NULL, "; // "Name#XLinkID#XLinkID, ..."
		$strQuery .= "Protection char(16) NOT NULL, ";
		$strQuery .= "MetaType char(16) NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( TypeID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "FOREIGN KEY ( ParentNamespaceID ) REFERENCES _PHP_CodeDoc_Namespaces( NamespaceID ), ";
		$strQuery .= "FOREIGN KEY ( ParentTypeID ) REFERENCES _PHP_CodeDoc_Types( TypeID ), ";
		$strQuery .= "UNIQUE ( ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );
		
		// Enums table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Enums ( ";
		$strQuery .= "EnumID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentNamespaceID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentTypeID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "Protection char(16) NOT NULL, ";
		$strQuery .= "EnumValues text NOT NULL, "; // "Name#XLinkID, ..."
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( EnumID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "FOREIGN KEY ( ParentNamespaceID ) REFERENCES _PHP_CodeDoc_Namespaces( NamespaceID ), ";
		$strQuery .= "FOREIGN KEY ( ParentTypeID ) REFERENCES _PHP_CodeDoc_Types( TypeID ), ";
		$strQuery .= "UNIQUE ( ParentFileID, ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Typedefs table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Typedefs ( ";
		$strQuery .= "TypedefID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentNamespaceID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentTypeID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "Protection char(16) NOT NULL, ";
		$strQuery .= "TargetTypeLink int UNSIGNED NOT NULL, ";
		$strQuery .= "Params tinytext NOT NULL, "; // "Name#XLinkID#XLinkID, ..."
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( TypedefID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "FOREIGN KEY ( ParentNamespaceID ) REFERENCES _PHP_CodeDoc_Namespaces( NamespaceID ), ";
		$strQuery .= "FOREIGN KEY ( ParentTypeID ) REFERENCES _PHP_CodeDoc_Types( TypeID ), ";
		$strQuery .= "UNIQUE ( ParentFileID, ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Variables table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Variables ( ";
		$strQuery .= "VariableID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentNamespaceID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentTypeID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "TemplateParams tinytext NOT NULL, "; // "Name#XLinkID#XLinkID, ..."
		$strQuery .= "Protection char(16) NOT NULL, ";
		$strQuery .= "Static tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Const tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Volatile tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Mutable tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "InstanceTypeLink int UNSIGNED NOT NULL, ";
		$strQuery .= "InitializeLink int UNSIGNED NOT NULL, ";
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( VariableID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "FOREIGN KEY ( ParentNamespaceID ) REFERENCES _PHP_CodeDoc_Namespaces( NamespaceID ), ";
		$strQuery .= "FOREIGN KEY ( ParentTypeID ) REFERENCES _PHP_CodeDoc_Types( TypeID ), ";
		$strQuery .= "UNIQUE ( ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Functions table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_Functions ( ";
		$strQuery .= "FunctionID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentFileID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentNamespaceID int UNSIGNED NOT NULL, ";
		$strQuery .= "ParentTypeID int UNSIGNED NOT NULL, ";
		$strQuery .= "ScopedNameID char(255) NOT NULL, ";
		$strQuery .= "Name tinytext NOT NULL, ";
		$strQuery .= "FileName tinytext NOT NULL, ";
		$strQuery .= "LineNumber int UNSIGNED NOT NULL, ";
		$strQuery .= "TemplateParams tinytext NOT NULL, "; // "Name#XLinkID#XLinkID, ..."
		$strQuery .= "Protection char(16) NOT NULL, ";
		$strQuery .= "Inline tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Static tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Virtual char(16) NOT NULL, ";
		$strQuery .= "Const tinyint UNSIGNED NOT NULL, ";		
		$strQuery .= "ReturnTypeLink int UNSIGNED NOT NULL, ";
		$strQuery .= "Params tinytext NOT NULL, "; 		   // "Name#XLinkID#XLinkID, ..."
		$strQuery .= "ShortDesc tinytext NOT NULL, ";
		$strQuery .= "DocContent text NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( FunctionID ), ";
		$strQuery .= "FOREIGN KEY ( ParentFileID ) REFERENCES _PHP_CodeDoc_Files( FileID ), ";
		$strQuery .= "FOREIGN KEY ( ParentNamespaceID ) REFERENCES _PHP_CodeDoc_Namespaces( NamespaceID ), ";
		$strQuery .= "FOREIGN KEY ( ParentTypeID ) REFERENCES _PHP_CodeDoc_Types( TypeID ), ";
		$strQuery .= "UNIQUE ( ScopedNameID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Crossed links table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_CrossedLinks ( ";
		$strQuery .= "XLinkID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "LinkText tinytext NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( XLinkID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Crossed references table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_CodeDoc_CrossedReferences ( ";
		$strQuery .= "XReferenceID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ParentXLinkID int UNSIGNED NOT NULL, ";
		$strQuery .= "TargetKind char(16) NOT NULL, ";
		$strQuery .= "TargetID int UNSIGNED NOT NULL, ";
		$strQuery .= "ReferenceText tinytext NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( XReferenceID ), ";
		$strQuery .= "FOREIGN KEY ( ParentXLinkID ) REFERENCES _PHP_CodeDoc_CrossedLinks( XLinkID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Documentation elements
		$arrTemp = array( "Root", "GettingStarted", "CodingConventions", "BasicFramework", "DevelopperManual", "Reference" );
		foreach( $arrTemp as $strDoc ) {
			$hSQL->Select( "_PHP_CodeDoc_Documentation", array( "Name" ), "Name='$strDoc'" );
			if ( $hSQL->GetRowCount() == 0 ) {
				$hSQL->Insert( "_PHP_CodeDoc_Documentation", array( "Name" => $strDoc,
																	"ShortDesc" => "Not imported yet ...",
																	"DocContent" => "Not imported yet ..." ) );
			}
		}

		// Null elements
		$iNullID = CODEDOC_ID_NULL;

		$hSQL->Select( "_PHP_CodeDoc_Directories", array( "DirectoryID" ), "DirectoryID=$iNullID" );
		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->Insert( "_PHP_CodeDoc_Directories", array( "DirectoryID" => CODEDOC_ID_NULL,
															  "ParentDirID" => CODEDOC_ID_NULL,
															  "ScopedNameID" => "",
															  "Name" => "",
															  "Path" => "",
															  "ShortDesc" => "Null",
															  "DocContent" => "Null" ) );
		}
		$hSQL->Select( "_PHP_CodeDoc_Files", array( "FileID" ), "FileID=$iNullID" );
		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->Insert( "_PHP_CodeDoc_Files", array( "FileID" => CODEDOC_ID_NULL,
														"ParentDirID" => CODEDOC_ID_NULL,
														"ScopedNameID" => "",
														"Name" => "",
														"Path" => "",
														"ShortDesc" => "Null",
														"DocContent" => "Null" ) );
		}
		$hSQL->Select( "_PHP_CodeDoc_Namespaces", array( "NamespaceID" ), "NamespaceID=$iNullID" );
		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->Insert( "_PHP_CodeDoc_Namespaces", array( "NamespaceID" => CODEDOC_ID_NULL,
															 "ParentFileID" => CODEDOC_ID_NULL,
															 "ParentNamespaceID" => CODEDOC_ID_NULL,
															 "ScopedNameID" => "",
															 "Name" => "",
															 "FileName" => "",
															 "LineNumber" => 0,
															 "Protection" => "public",
															 "ShortDesc" => "Null",
															 "DocContent" => "Null" ) );
		}
		$hSQL->Select( "_PHP_CodeDoc_Types", array( "TypeID" ), "TypeID=$iNullID" );
		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->Insert( "_PHP_CodeDoc_Types", array( "TypeID" => CODEDOC_ID_NULL,
														"ParentFileID" => CODEDOC_ID_NULL,
														"ParentNamespaceID" => CODEDOC_ID_NULL,
														"ParentTypeID" => CODEDOC_ID_NULL,
														"ScopedNameID" => "",
														"Name" => "",
														"FileName" => "",
														"LineNumber" => 0,
														"BaseTypes" => "",
														"TemplateParams" => "",
														"Protection" => "public",
														"MetaType" => "interface",
														"ShortDesc" => "Null",
														"DocContent" => "Null" ) );
		}

		$hSQL->DisconnectTemporary();
	}

		// Update routines
	private function _Update( & $strEvent, & $strEventParam )
	{
		switch( $strEvent ) {
			case CODEDOC_EVENT_ADMIN_GENERATE:
				$this->GenerateStructure();
				$this->ImportContent();

				$strEvent = CODEDOC_DISPLAY_ROOT;
				$strEventParam = "";
				break;
			case CODEDOC_EVENT_ADMIN_UPDATE:
				$this->ImportContent();

				$strEvent = CODEDOC_DISPLAY_ROOT;
				$strEventParam = "";
				break;
			default:
				// nothing to do
				break;
		}
	}

		// Display routines : helpers
	private function _DisplayChildLinks( $arrLinks, $strSourceMark, $strLinkMark )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$iLinkCount = count( $arrLinks );

		for( $i = 0; $i < $iLinkCount; ++$i ) {
			$arrLinkInfos = $arrLinks[$i];
			$iLinkID = $arrLinkInfos[$strLinkMark."ID"];
			$strLinkName = $arrLinkInfos["Name"];

			$hHTML->SetID( "CodeDocHeader_NavigationTree_$strSourceMark"."Link_$strLinkMark"."_$iLinkID" );
			$hHTML->SetTooltip( $strLinkName );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ChildLink" );
			$hAJAX->SetEventTags (
				HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
				array( array( "'Display$strLinkMark'", "'$iLinkID'" ), array( "'Display$strLinkMark'", "'$iLinkID'" ) )
			);
			$hHTML->DivStart();
				$hHTML->Text( $strLinkName );
			$hHTML->DivEnd();
		}
	}
	private function _DisplayCrossedLink( $iXLinkID )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$arrXLinkInfos = $this->GetXLinkInfos( $iXLinkID );
		$strLinkText = $arrXLinkInfos["LinkText"];

		$arrXReferences = $this->ListXReferences( $iXLinkID, true );
		$iXReferenceCount = count( $arrXReferences );
		for( $i = 0; $i < $iXReferenceCount; ++$i ) {
			$arrXRefInfos = $arrXReferences[$i];
			$strTargetKind = $arrXRefInfos["TargetKind"];
			$iTargetID = $arrXRefInfos["TargetID"];
			$strRefText = $arrXRefInfos["ReferenceText"];

			$iPos = strpos( $strLinkText, $strRefText );
			$strLeft = substr( $strLinkText, 0, $iPos );
			$strRight = substr( $strLinkText, $iPos + strlen($strRefText) );

			$hHTML->Text( $strLeft );

			$hHTML->SetID( "CodeDocContent_CrossedLink_$strTargetKind"."_$iTargetID" );
			$hHTML->SetStyleClass( "CodeDocProperties_CrossedLink" );
			$hAJAX->SetEventTags (
				HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
				array( array( "'Display$strTargetKind'", "'$iTargetID'" ), array( "'Display$strTargetKind'", "'$iTargetID'" ) )
			);
			$hHTML->SpanStart();
				$hHTML->Text( "$strRefText" );
			$hHTML->SpanEnd();

			$strLinkText = $strRight;
		}

		$hHTML->Text( $strLinkText );
	}
	public function _DisplayParams( $arrParams )
	{
		$hHTML = HTMLManager::GetSingleton();

		$iParamCount = count( $arrParams );
		for( $i = 0; $i < $iParamCount; ++$i ) {
			$arrParamInfos = $arrParams[$i];
			$this->_DisplayCrossedLink( $arrParamInfos[1] );
			$hHTML->Text( $arrParamInfos[0] );
			$arrXLinkInfos = $this->GetXLinkInfos( $arrParamInfos[2] );
			if ( $arrXLinkInfos["LinkText"] != "" ) {
				$hHTML->Text( " = " );
				$this->_DisplayCrossedLink( $arrParamInfos[2] );
			}
			if ( $i < $iParamCount - 1 )
				$hHTML->Text( ", " );
		}
	}

		// Display routines : CodeDocHeader
	private function _DisplayGenerator()
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();

		$bValidSession = $hSession->Open();

		// Admin user only
		if ( $bValidSession && $hSession->IsAdmin() ) {
			$hHTML->SetID( "CodeDocHeader_Commands_Generator" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H" );
			$hHTML->DivStart();
				$hHTML->SetID( "CodeDocHeader_GeneratorButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
					array( array( "'AdminGenerate'", "''" ), array( "'DisplayRoot'", "''" ) )
				);
				$hHTML->InputButton( "Generate" );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocHeader_Commands_Updater" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H" );
			$hHTML->DivStart();
				$hHTML->SetID( "CodeDocHeader_UpdaterButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
					array( array( "'AdminUpdate'", "''" ), array( "'DisplayRoot'", "''" ) )
				);
				$hHTML->InputButton( "Update" );
			$hHTML->DivEnd();
		}
		
		if ( $bValidSession )
			$hSession->Close();
	}

	private function _DisplayNavigationTree( $strEvent, $strEventParam )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		// Rebuild path to root
		$arrNavigationPath = array();

		$strCurrentKind = $strEvent;
		$iCurrentID = intval( $strEventParam );

		$bDone = false;
		while( !$bDone ) {
			switch( $strCurrentKind ) {
				case CODEDOC_DISPLAY_ROOT:
					array_push( $arrNavigationPath, array( "Root" ) );
					$bDone = true;
					break;				
				case CODEDOC_DISPLAY_GETTINGSTARTED:
					array_push( $arrNavigationPath, array( "GettingStarted", "Getting Started" ) );
					$strCurrentKind = CODEDOC_DISPLAY_ROOT;
					break;
				case CODEDOC_DISPLAY_CONVENTIONS:
					array_push( $arrNavigationPath, array( "Conventions", "Coding Conventions" ) );
					$strCurrentKind = CODEDOC_DISPLAY_ROOT;
					break;
				case CODEDOC_DISPLAY_FRAMEWORK:
					array_push( $arrNavigationPath, array( "Framework", "Basic Framework" ) );
					$strCurrentKind = CODEDOC_DISPLAY_ROOT;
					break;
				case CODEDOC_DISPLAY_MANUAL:
					array_push( $arrNavigationPath, array( "Manual", "Developper Manual" ) );
					$strCurrentKind = CODEDOC_DISPLAY_ROOT;
					break;
				case CODEDOC_DISPLAY_REFERENCE:
					array_push( $arrNavigationPath, array( "Reference", "API Reference" ) );
					$strCurrentKind = CODEDOC_DISPLAY_ROOT;
					break;
				case CODEDOC_DISPLAY_DIRECTORY:
					$arrDirectoryInfos = $this->GetDirectoryInfos( $iCurrentID );
					$iParentDirID = $arrDirectoryInfos["ParentDirID"];
					$strDirectoryName = $arrDirectoryInfos["Name"];

					array_push( $arrNavigationPath, array( "Directory", $iCurrentID, $strDirectoryName ) );
					if ( $iParentDirID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_DIRECTORY;
						$iCurrentID = $iParentDirID;
					} else {
						$strCurrentKind = CODEDOC_DISPLAY_REFERENCE;
						$iCurrentID = CODEDOC_ID_NULL;
					}
					break;
				case CODEDOC_DISPLAY_FILE:
					$arrFileInfos = $this->GetFileInfos( $iCurrentID );
					$iParentDirID = $arrFileInfos["ParentDirID"];
					$strFileName = $arrFileInfos["Name"];

					array_push( $arrNavigationPath, array( "File", $iCurrentID, $strFileName ) );
					$strCurrentKind = CODEDOC_DISPLAY_DIRECTORY;
					$iCurrentID = $iParentDirID;
					break;
				case CODEDOC_DISPLAY_DEFINE:
					$arrDefineInfos = $this->GetDefineInfos( $iCurrentID );
					$iParentFileID = $arrDefineInfos["ParentFileID"];
					$strDefineName = $arrDefineInfos["Name"];

					array_push( $arrNavigationPath, array( "Define", $iCurrentID, $strDefineName ) );
					$strCurrentKind = CODEDOC_DISPLAY_FILE;
					$iCurrentID = $iParentFileID;
					break;
				case CODEDOC_DISPLAY_MACRO:
					$arrMacroInfos = $this->GetMacroInfos( $iCurrentID );
					$iParentFileID = $arrMacroInfos["ParentFileID"];
					$strMacroName = $arrMacroInfos["Name"];

					array_push( $arrNavigationPath, array( "Macro", $iCurrentID, $strMacroName ) );
					$strCurrentKind = CODEDOC_DISPLAY_FILE;
					$iCurrentID = $iParentFileID;
					break;
				case CODEDOC_DISPLAY_NAMESPACE:
					$arrNamespaceInfos = $this->GetNamespaceInfos( $iCurrentID );
					$iParentFileID = $arrNamespaceInfos["ParentFileID"];
					$iParentNamespaceID = $arrNamespaceInfos["ParentNamespaceID"];
					$strNamespaceName = $arrNamespaceInfos["Name"];

					array_push( $arrNavigationPath, array( "Namespace", $iCurrentID, $strNamespaceName ) );
					if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_NAMESPACE;
						$iCurrentID = $iParentNamespaceID;
					} else {
						assert( '$iParentFileID != CODEDOC_ID_NULL' );
						$strCurrentKind = CODEDOC_DISPLAY_FILE;
						$iCurrentID = $iParentFileID;
					}
					break;
				case CODEDOC_DISPLAY_TYPE:
					$arrTypeInfos = $this->GetTypeInfos( $iCurrentID );
					$iParentFileID = $arrTypeInfos["ParentFileID"];
					$iParentNamespaceID = $arrTypeInfos["ParentNamespaceID"];
					$iParentTypeID = $arrTypeInfos["ParentTypeID"];
					$strTypeName = $arrTypeInfos["Name"];

					array_push( $arrNavigationPath, array( "Type", $iCurrentID, $strTypeName ) );
					if ( $iParentTypeID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_TYPE;
						$iCurrentID = $iParentTypeID;
					} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_NAMESPACE;
						$iCurrentID = $iParentNamespaceID;
					} else {
						assert( '$iParentFileID != CODEDOC_ID_NULL' );
						$strCurrentKind = CODEDOC_DISPLAY_FILE;
						$iCurrentID = $iParentFileID;
					}
					break;
				case CODEDOC_DISPLAY_ENUM:
					$arrEnumInfos = $this->GetEnumInfos( $iCurrentID );
					$iParentFileID = $arrEnumInfos["ParentFileID"];
					$iParentNamespaceID = $arrEnumInfos["ParentNamespaceID"];
					$iParentTypeID = $arrEnumInfos["ParentTypeID"];
					$strEnumName = $arrEnumInfos["Name"];

					array_push( $arrNavigationPath, array( "Enum", $iCurrentID, $strEnumName ) );
					if ( $iParentTypeID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_TYPE;
						$iCurrentID = $iParentTypeID;
					} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_NAMESPACE;
						$iCurrentID = $iParentNamespaceID;
					} else {
						assert( '$iParentFileID != CODEDOC_ID_NULL' );
						$strCurrentKind = CODEDOC_DISPLAY_FILE;
						$iCurrentID = $iParentFileID;
					}
					break;
				case CODEDOC_DISPLAY_TYPEDEF:
					$arrTypedefInfos = $this->GetTypedefInfos( $iCurrentID );
					$iParentFileID = $arrTypedefInfos["ParentFileID"];
					$iParentNamespaceID = $arrTypedefInfos["ParentNamespaceID"];
					$iParentTypeID = $arrTypedefInfos["ParentTypeID"];
					$strTypedefName = $arrTypedefInfos["Name"];

					array_push( $arrNavigationPath, array( "Typedef", $iCurrentID, $strTypedefName ) );
					if ( $iParentTypeID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_TYPE;
						$iCurrentID = $iParentTypeID;
					} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_NAMESPACE;
						$iCurrentID = $iParentNamespaceID;
					} else {
						assert( '$iParentFileID != CODEDOC_ID_NULL' );
						$strCurrentKind = CODEDOC_DISPLAY_FILE;
						$iCurrentID = $iParentFileID;
					}
					break;
				case CODEDOC_DISPLAY_VARIABLE:
					$arrVariableInfos = $this->GetVariableInfos( $iCurrentID );
					$iParentFileID = $arrVariableInfos["ParentFileID"];
					$iParentNamespaceID = $arrVariableInfos["ParentNamespaceID"];
					$iParentTypeID = $arrVariableInfos["ParentTypeID"];
					$strVariableName = $arrVariableInfos["Name"];

					array_push( $arrNavigationPath, array( "Variable", $iCurrentID, $strVariableName ) );
					if ( $iParentTypeID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_TYPE;
						$iCurrentID = $iParentTypeID;
					} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_NAMESPACE;
						$iCurrentID = $iParentNamespaceID;
					} else {
						assert( '$iParentFileID != CODEDOC_ID_NULL' );
						$strCurrentKind = CODEDOC_DISPLAY_FILE;
						$iCurrentID = $iParentFileID;
					}
					break;
				case CODEDOC_DISPLAY_FUNCTION:
					$arrFunctionInfos = $this->GetFunctionInfos( $iCurrentID );
					$iParentFileID = $arrFunctionInfos["ParentFileID"];
					$iParentNamespaceID = $arrFunctionInfos["ParentNamespaceID"];
					$iParentTypeID = $arrFunctionInfos["ParentTypeID"];
					$strFunctionName = $arrFunctionInfos["Name"];

					array_push( $arrNavigationPath, array( "Function", $iCurrentID, $strFunctionName ) );
					if ( $iParentTypeID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_TYPE;
						$iCurrentID = $iParentTypeID;
					} else if ( $iParentNamespaceID != CODEDOC_ID_NULL ) {
						$strCurrentKind = CODEDOC_DISPLAY_NAMESPACE;
						$iCurrentID = $iParentNamespaceID;
					} else {
						assert( '$iParentFileID != CODEDOC_ID_NULL' );
						$strCurrentKind = CODEDOC_DISPLAY_FILE;
						$iCurrentID = $iParentFileID;
					}
					break;
				default: assert('false'); break;
			}
		}

		// Display navigation path
		$iPathLength = count( $arrNavigationPath );
		assert( '$iPathLength > 0' );

		// Walk the path backward
		for( $i = $iPathLength - 1; $i >= 0; --$i ) {
			$arrPathNode = $arrNavigationPath[$i];
			$strKindMark = $arrPathNode[0];

			if ( count($arrPathNode) < 3 ) {
				$strNodeName = $strKindMark;
				if ( count($arrPathNode) == 2 )
					$strNodeName = $arrPathNode[1];

				$hHTML->SetID( "CodeDocHeader_NavigationTree_ParentLink_$strKindMark" );
				$hHTML->SetTooltip( $strNodeName );
				if ( $i == 0 )
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_NodeLink" );
				else
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ParentLink" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
					array( array( "'Display$strKindMark'", "''" ), array( "'Display$strKindMark'", "''" ) )
				);
				$hHTML->DivStart();
					$hHTML->Text( $strNodeName );
				$hHTML->DivEnd();
			} else {
				assert( 'count($arrPathNode) == 3' );
				$iNodeID = $arrPathNode[1];
				$strNodeName = $arrPathNode[2];
				
				$hHTML->SetID( "CodeDocHeader_NavigationTree_ParentLink_$strKindMark"."_$iNodeID" );
				$hHTML->SetTooltip( $strNodeName );
				if ( $i == 0 )
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_NodeLink" );
				else
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ParentLink" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
					array( array( "'Display$strKindMark'", "'$iNodeID'" ), array( "'Display$strKindMark'", "'$iNodeID'" ) )
				);
				$hHTML->DivStart();
					$hHTML->Text( $strNodeName );
				$hHTML->DivEnd();
			}
		}

		// Last node's children
		$strCurrentKind = $strEvent;
		$iCurrentID = intval( $strEventParam );
		switch( $strCurrentKind ) {
			case CODEDOC_DISPLAY_ROOT:      $this->_DisplayRootLinks();                   break;
			case CODEDOC_DISPLAY_MANUAL: 	$this->_DisplayManualLinks();              	  break;
			case CODEDOC_DISPLAY_REFERENCE: $this->_DisplayReferenceLinks();              break;
			case CODEDOC_DISPLAY_DIRECTORY: $this->_DisplayDirectoryLinks( $iCurrentID ); break;
			case CODEDOC_DISPLAY_FILE:      $this->_DisplayFileLinks( $iCurrentID );      break;
			case CODEDOC_DISPLAY_NAMESPACE: $this->_DisplayNamespaceLinks( $iCurrentID ); break;
			case CODEDOC_DISPLAY_TYPE:      $this->_DisplayTypeLinks( $iCurrentID );      break;
			default: break;
		}
	}

	private function _DisplayRootLinks()
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		
		$hHTML->SetID( "CodeDocHeader_NavigationTree_RootLink_GettingStarted" );
		$hHTML->SetTooltip( "Getting Started" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ChildLink" );
		$hAJAX->SetEventTags (
			HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
			array( array( "'DisplayGettingStarted'", "''" ), array( "'DisplayGettingStarted'", "''" ) )
		);
		$hHTML->DivStart();
			$hHTML->Text( "Getting Started" );
		$hHTML->DivEnd();

		$hHTML->SetID( "CodeDocHeader_NavigationTree_RootLink_Conventions" );
		$hHTML->SetTooltip( "Conventions" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ChildLink" );
		$hAJAX->SetEventTags (
			HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
			array( array( "'DisplayConventions'", "''" ), array( "'DisplayConventions'", "''" ) )
		);
		$hHTML->DivStart();
			$hHTML->Text( "Coding Conventions" );
		$hHTML->DivEnd();

		$hHTML->SetID( "CodeDocHeader_NavigationTree_RootLink_Framework" );
		$hHTML->SetTooltip( "Framework" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ChildLink" );
		$hAJAX->SetEventTags (
			HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
			array( array( "'DisplayFramework'", "''" ), array( "'DisplayFramework'", "''" ) )
		);
		$hHTML->DivStart();
			$hHTML->Text( "Basic Framework" );
		$hHTML->DivEnd();

		$hHTML->SetID( "CodeDocHeader_NavigationTree_RootLink_Manual" );
		$hHTML->SetTooltip( "Developper Manual" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ChildLink" );
		$hAJAX->SetEventTags (
			HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
			array( array( "'DisplayManual'", "''" ), array( "'DisplayManual'", "''" ) )
		);
		$hHTML->DivStart();
			$hHTML->Text( "Developper Manual" );
		$hHTML->DivEnd();
		
		$hHTML->SetID( "CodeDocHeader_NavigationTree_RootLink_Reference" );
		$hHTML->SetTooltip( "API Reference" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Link CodeDocProperties_NavTree_ChildLink" );
		$hAJAX->SetEventTags (
			HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
			array( array( "'DisplayReference'", "''" ), array( "'DisplayReference'", "''" ) )
		);
		$hHTML->DivStart();
			$hHTML->Text( "API Reference" );
		$hHTML->DivEnd();
	}
	private function _DisplayManualLinks()
	{
///////////////////////////////////////////////////////////////////
	}
	private function _DisplayReferenceLinks()
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrDirectories = $this->ListDirectories( CODEDOC_ID_NULL, true );

		$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
		$hHTML->DivStart();
			$hHTML->Text( "Root Directories :" );
		$hHTML->DivEnd();
		$this->_DisplayChildLinks( $arrDirectories, "Reference", "Directory" );
	}
	private function _DisplayDirectoryLinks( $iDirectoryID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrSubDirectories  = $this->ListDirectories( $iDirectoryID, true );
		$arrFiles           = $this->ListFiles( $iDirectoryID, true );

		if ( count($arrSubDirectories) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Sub-Directories :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrSubDirectories, "Directory", "Directory" );
		}

		if ( count($arrFiles) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Files :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrFiles, "Directory", "File" );
		}
	}
	private function _DisplayFileLinks( $iFileID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrDefines     = $this->ListDefines( $iFileID, true );
		$arrMacros      = $this->ListMacros( $iFileID, true );
		$arrEnums   	= $this->ListGlobalEnums( $iFileID, true );
		$arrTypedefs    = $this->ListGlobalTypedefs( $iFileID, true );
		$arrVariables   = $this->ListGlobalVariables( $iFileID, true );
		$arrFunctions   = $this->ListGlobalFunctions( $iFileID, true );
		$arrNamespaces  = $this->ListGlobalNamespaces( $iFileID, true );
		$arrTypes       = $this->ListGlobalTypes( $iFileID, true );

		if ( count($arrDefines) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Defines :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrDefines, "File", "Define" );
		}

		if ( count($arrMacros) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Macros :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrMacros, "File", "Macro" );
		}

		if ( count($arrEnums) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Global Enums :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrEnums, "File", "Enum" );
		}

		if ( count($arrTypedefs) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Global Typedefs :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrTypedefs, "File", "Typedef" );
		}

		if ( count($arrVariables) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Global Variables :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrVariables, "File", "Variable" );
		}

		if ( count($arrFunctions) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Global Functions :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrFunctions, "File", "Function" );
		}

		if ( count($arrNamespaces) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Global Namespaces :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrNamespaces, "File", "Namespace" );
		}

		if ( count($arrTypes) > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Global Types :" );
			$hHTML->DivEnd();
			$this->_DisplayChildLinks( $arrTypes, "File", "Type" );
		}
	}
	private function _DisplayNamespaceLinks( $iNamespaceID )
	{
		$hHTML = HTMLManager::GetSingleton();
		
		$arrNestedEnumsPublic	 = $this->ListNamespaceEnums( $iNamespaceID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrNestedTypedefsPublic = $this->ListNamespaceTypedefs( $iNamespaceID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrNestedTypesPublic	 = $this->ListNamespaceTypes( $iNamespaceID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrMethodsPublic		 = $this->ListNamespaceFunctions( $iNamespaceID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrMembersPublic		 = $this->ListNamespaceVariables( $iNamespaceID, CODEDOC_PROTECTION_PUBLIC, true );

		$arrNestedEnumsProtected	= $this->ListNamespaceEnums( $iNamespaceID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrNestedTypedefsProtected = $this->ListNamespaceTypedefs( $iNamespaceID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrNestedTypesProtected	= $this->ListNamespaceTypes( $iNamespaceID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrMethodsProtected		= $this->ListNamespaceFunctions( $iNamespaceID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrMembersProtected		= $this->ListNamespaceVariables( $iNamespaceID, CODEDOC_PROTECTION_PROTECTED, true );

		$arrNestedEnumsPrivate	  = $this->ListNamespaceEnums( $iNamespaceID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrNestedTypedefsPrivate = $this->ListNamespaceTypedefs( $iNamespaceID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrNestedTypesPrivate	  = $this->ListNamespaceTypes( $iNamespaceID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrMethodsPrivate		  = $this->ListNamespaceFunctions( $iNamespaceID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrMembersPrivate		  = $this->ListNamespaceVariables( $iNamespaceID, CODEDOC_PROTECTION_PRIVATE, true );
		
		$arrCountsPublic    = array( count($arrNestedEnumsPublic), count($arrNestedTypedefsPublic), count($arrNestedTypesPublic),
									 count($arrMethodsPublic), count($arrMembersPublic) );
		$arrCountsProtected = array( count($arrNestedEnumsProtected), count($arrNestedTypedefsProtected), count($arrNestedTypesProtected),
									 count($arrMethodsProtected), count($arrMembersProtected) );
		$arrCountsPrivate   = array( count($arrNestedEnumsPrivate), count($arrNestedTypedefsPrivate), count($arrNestedTypesPrivate),
									 count($arrMethodsPrivate), count($arrMembersPrivate) );

		if ( $arrCountsPublic[0] > 0 || $arrCountsPublic[1] > 0 || $arrCountsPublic[2] > 0 ||
			 $arrCountsPublic[3] > 0 || $arrCountsPublic[4] > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level0Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Public Content :" );
			$hHTML->DivEnd();
	
				if ( $arrCountsPublic[0] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Enums :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedEnumsPublic, "Namespace", "Enum" );
				}
	
				if ( $arrCountsPublic[1] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Typedefs :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypedefsPublic, "Namespace", "Typedef" );
				}
	
				if ( $arrCountsPublic[2] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Types :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypesPublic, "Namespace", "Type" );
				}
	
				if ( $arrCountsPublic[3] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Methods :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMethodsPublic, "Namespace", "Function" );
				}
	
				if ( $arrCountsPublic[4] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Members :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMembersPublic, "Namespace", "Variable" );
				}
		}

		if ( $arrCountsProtected[0] > 0 || $arrCountsProtected[1] > 0 || $arrCountsProtected[2] > 0 ||
			 $arrCountsProtected[3] > 0 || $arrCountsProtected[4] > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level0Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Protected Content :" );
			$hHTML->DivEnd();
	
				if ( $arrCountsProtected[0] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Enums :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedEnumsProtected, "Namespace", "Enum" );
				}
	
				if ( $arrCountsProtected[1] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Typedefs :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypedefsProtected, "Namespace", "Typedef" );
				}
	
				if ( $arrCountsProtected[2] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Types :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypesProtected, "Namespace", "Type" );
				}
	
				if ( $arrCountsProtected[3] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Methods :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMethodsProtected, "Namespace", "Function" );
				}
	
				if ( $arrCountsProtected[4] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Members :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMembersProtected, "Namespace", "Variable" );
				}
		}

		if ( $arrCountsPrivate[0] > 0 || $arrCountsPrivate[1] > 0 || $arrCountsPrivate[2] > 0 ||
			 $arrCountsPrivate[3] > 0 || $arrCountsPrivate[4] > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level0Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Private Content :" );
			$hHTML->DivEnd();
	
				if ( $arrCountsPrivate[0] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Enums :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedEnumsPrivate, "Namespace", "Enum" );
				}
	
				if ( $arrCountsPrivate[1] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Typedefs :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypedefsPrivate, "Namespace", "Typedef" );
				}
	
				if ( $arrCountsPrivate[2] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Types :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypesPrivate, "Namespace", "Type" );
				}
	
				if ( $arrCountsPrivate[3] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Methods :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMethodsPrivate, "Namespace", "Function" );
				}
	
				if ( $arrCountsPrivate[4] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Members :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMembersPrivate, "Namespace", "Variable" );
				}
		}
	}
	private function _DisplayTypeLinks( $iTypeID )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$arrTypeInfos = $this->GetTypeInfos( $iTypeID );
		$arrBaseTypes = $arrTypeInfos["BaseTypes"];

		$arrDerivedTypes = $this->ListDerivedTypes( $iTypeID, true );

		$arrNestedEnumsPublic	 = $this->ListNestedEnums( $iTypeID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrNestedTypedefsPublic = $this->ListNestedTypedefs( $iTypeID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrNestedTypesPublic	 = $this->ListNestedTypes( $iTypeID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrMethodsPublic		 = $this->ListNestedFunctions( $iTypeID, CODEDOC_PROTECTION_PUBLIC, true );
		$arrMembersPublic		 = $this->ListNestedVariables( $iTypeID, CODEDOC_PROTECTION_PUBLIC, true );

		$arrNestedEnumsProtected	= $this->ListNestedEnums( $iTypeID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrNestedTypedefsProtected = $this->ListNestedTypedefs( $iTypeID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrNestedTypesProtected	= $this->ListNestedTypes( $iTypeID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrMethodsProtected		= $this->ListNestedFunctions( $iTypeID, CODEDOC_PROTECTION_PROTECTED, true );
		$arrMembersProtected		= $this->ListNestedVariables( $iTypeID, CODEDOC_PROTECTION_PROTECTED, true );

		$arrNestedEnumsPrivate	  = $this->ListNestedEnums( $iTypeID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrNestedTypedefsPrivate = $this->ListNestedTypedefs( $iTypeID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrNestedTypesPrivate	  = $this->ListNestedTypes( $iTypeID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrMethodsPrivate		  = $this->ListNestedFunctions( $iTypeID, CODEDOC_PROTECTION_PRIVATE, true );
		$arrMembersPrivate		  = $this->ListNestedVariables( $iTypeID, CODEDOC_PROTECTION_PRIVATE, true );
		
		$arrCountsInheritance = array( count($arrBaseTypes), count($arrDerivedTypes) );
		$arrCountsPublic    = array( count($arrNestedEnumsPublic), count($arrNestedTypedefsPublic), count($arrNestedTypesPublic),
									 count($arrMethodsPublic), count($arrMembersPublic) );
		$arrCountsProtected = array( count($arrNestedEnumsProtected), count($arrNestedTypedefsProtected), count($arrNestedTypesProtected),
									 count($arrMethodsProtected), count($arrMembersProtected) );
		$arrCountsPrivate   = array( count($arrNestedEnumsPrivate), count($arrNestedTypedefsPrivate), count($arrNestedTypesPrivate),
									 count($arrMethodsPrivate), count($arrMembersPrivate) );

		if ( $arrCountsInheritance[0] > 0 || $arrCountsInheritance[1] > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level0Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Inheritance links :" );
			$hHTML->DivEnd();
	
				if ( $arrCountsInheritance[0] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Base Types :" );
					$hHTML->DivEnd();
					foreach( $arrBaseTypes as $iBaseTypeID => $strDerivMethod ) {
						$arrBaseTypeInfos = $this->GetTypeInfos( $iBaseTypeID );
						$strBaseTypeName = $arrBaseTypeInfos["Name"];
			
						$hHTML->SetID( "CodeDocHeader_NavigationTree_TypeLink_Type_$iBaseTypeID" );
						$hHTML->SetTooltip( "$strBaseTypeName ($strDerivMethod derivation)" );
						$hHTML->SetStyleClass( "CodeDocHeader_NavigationTree_ChildClass" );
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "CodeDocContent", "CodeDocHeader" ),
							array( array( "'DisplayType'", "'$iBaseTypeID'" ), array( "'DisplayType'", "'$iBaseTypeID'" ) )
						);
						$hHTML->DivStart();
							$hHTML->Text( "$strBaseTypeName ($strDerivMethod derivation)" );
						$hHTML->DivEnd();
					}
				}
	
				if ( $arrCountsInheritance[1] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Derived Types :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrDerivedTypes, "Type", "Type" );
				}
		}

		if ( $arrCountsPublic[0] > 0 || $arrCountsPublic[1] > 0 || $arrCountsPublic[2] > 0 ||
			 $arrCountsPublic[3] > 0 || $arrCountsPublic[4] > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level0Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Public Content :" );
			$hHTML->DivEnd();
	
				if ( $arrCountsPublic[0] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Enums :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedEnumsPublic, "Type", "Enum" );
				}
	
				if ( $arrCountsPublic[1] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Typedefs :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypedefsPublic, "Type", "Typedef" );
				}
	
				if ( $arrCountsPublic[2] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Types :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypesPublic, "Type", "Type" );
				}
	
				if ( $arrCountsPublic[3] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Methods :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMethodsPublic, "Type", "Function" );
				}
	
				if ( $arrCountsPublic[4] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Members :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMembersPublic, "Type", "Variable" );
				}
		}

		if ( $arrCountsProtected[0] > 0 || $arrCountsProtected[1] > 0 || $arrCountsProtected[2] > 0 ||
			 $arrCountsProtected[3] > 0 || $arrCountsProtected[4] > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level0Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Protected Content :" );
			$hHTML->DivEnd();
	
				if ( $arrCountsProtected[0] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Enums :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedEnumsProtected, "Type", "Enum" );
				}
	
				if ( $arrCountsProtected[1] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Typedefs :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypedefsProtected, "Type", "Typedef" );
				}
	
				if ( $arrCountsProtected[2] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Types :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypesProtected, "Type", "Type" );
				}
	
				if ( $arrCountsProtected[3] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Methods :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMethodsProtected, "Type", "Function" );
				}
	
				if ( $arrCountsProtected[4] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Members :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMembersProtected, "Type", "Variable" );
				}
		}

		if ( $arrCountsPrivate[0] > 0 || $arrCountsPrivate[1] > 0 || $arrCountsPrivate[2] > 0 ||
			 $arrCountsPrivate[3] > 0 || $arrCountsPrivate[4] > 0 ) {
			$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level0Text" );
			$hHTML->DivStart();
				$hHTML->Text( "Private Content :" );
			$hHTML->DivEnd();
	
				if ( $arrCountsPrivate[0] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Enums :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedEnumsPrivate, "Type", "Enum" );
				}
	
				if ( $arrCountsPrivate[1] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Typedefs :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypedefsPrivate, "Type", "Typedef" );
				}
	
				if ( $arrCountsPrivate[2] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Nested Types :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrNestedTypesPrivate, "Type", "Type" );
				}
	
				if ( $arrCountsPrivate[3] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Methods :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMethodsPrivate, "Type", "Function" );
				}
	
				if ( $arrCountsPrivate[4] > 0 ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level2H CodeDocProperties_NavTree_Text CodeDocProperties_NavTree_Level1Text" );
					$hHTML->DivStart();
						$hHTML->Text( "Members :" );
					$hHTML->DivEnd();
					$this->_DisplayChildLinks( $arrMembersPrivate, "Type", "Variable" );
				}
		}
	}

		// Display routines : CodeDocContent
	private function _DisplayRoot()
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrRootInfos = $this->GetDocumentationInfos( "Root" );

		$strShortDesc = $arrRootInfos["ShortDesc"];
		$strDocContent = $arrRootInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_Root" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_Root_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_Root_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_Root_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_Root_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayGettingStarted()
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrGettingStartedInfos = $this->GetDocumentationInfos( "GettingStarted" );

		$strShortDesc = $arrGettingStartedInfos["ShortDesc"];
		$strDocContent = $arrGettingStartedInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_GettingStarted" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_GettingStarted_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_GettingStarted_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_GettingStarted_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_GettingStarted_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayCodingConventions()
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrCodingConventionsInfos = $this->GetDocumentationInfos( "CodingConventions" );

		$strShortDesc = $arrCodingConventionsInfos["ShortDesc"];
		$strDocContent = $arrCodingConventionsInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_CodingConventions" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_CodingConventions_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_CodingConventions_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_CodingConventions_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_CodingConventions_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayBasicFramework()
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrBasicFrameworkInfos = $this->GetDocumentationInfos( "BasicFramework" );

		$strShortDesc = $arrBasicFrameworkInfos["ShortDesc"];
		$strDocContent = $arrBasicFrameworkInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_BasicFramework" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_BasicFramework_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_BasicFramework_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_BasicFramework_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_BasicFramework_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayDevelopperManual()
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrDevelopperManualInfos = $this->GetDocumentationInfos( "DevelopperManual" );

		$strShortDesc = $arrDevelopperManualInfos["ShortDesc"];
		$strDocContent = $arrDevelopperManualInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_DevelopperManual" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_DevelopperManual_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_DevelopperManual_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_DevelopperManual_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_DevelopperManual_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayReference()
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrReferenceInfos = $this->GetDocumentationInfos( "Reference" );

		$strShortDesc = $arrReferenceInfos["ShortDesc"];
		$strDocContent = $arrReferenceInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_Reference" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_Reference_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_Reference_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_Reference_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_Reference_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}

	private function _DisplayDirectory( $iDirectoryID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrDirectoryInfos = $this->GetDirectoryInfos( $iDirectoryID );
		$strName = $arrDirectoryInfos["Name"];
		$strPath = $arrDirectoryInfos["Path"];

		$strShortDesc = $arrDirectoryInfos["ShortDesc"];
		$strDocContent = $arrDirectoryInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefDirectory" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefDirectory_Name" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Name" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strName );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefDirectory_Path" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Path" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strPath );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefDirectory_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefDirectory_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefDirectory_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefDirectory_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayFile( $iFileID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrFileInfos = $this->GetFileInfos( $iFileID );
		$strName = $arrFileInfos["Name"];
		$strPath = $arrFileInfos["Path"];

		$strShortDesc = $arrFileInfos["ShortDesc"];
		$strDocContent = $arrFileInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefFile" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefFile_Name" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Name" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strName );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefFile_Path" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Path" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strPath );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefFile_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefFile_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefFile_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefFile_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}

	private function _DisplayDefine( $iDefineID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrDefineInfos = $this->GetDefineInfos( $iDefineID );
		$strName = $arrDefineInfos["Name"];
		$strFileName = $arrDefineInfos["FileName"];
		$iLineNumber = $arrDefineInfos["LineNumber"];

		$iBodyXLinkID = $arrDefineInfos["BodyLink"];

		$strShortDesc = $arrDefineInfos["ShortDesc"];
		$strDocContent = $arrDefineInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefDefine" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();			
			$hHTML->SetID( "CodeDocContent_RefDefine_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "#Define $strName " );
					$this->_DisplayCrossedLink( $iBodyXLinkID );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefDefine_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefDefine_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefDefine_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefDefine_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefDefine_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayMacro( $iMacroID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrMacroInfos = $this->GetMacroInfos( $iMacroID );
		$strName = $arrMacroInfos["Name"];
		$strFileName = $arrMacroInfos["FileName"];
		$iLineNumber = $arrMacroInfos["LineNumber"];

		$arrParams = $arrMacroInfos["Params"];
		$iBodyXLinkID = $arrMacroInfos["BodyLink"];

		$strShortDesc = $arrMacroInfos["ShortDesc"];
		$strDocContent = $arrMacroInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefMacro" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefMacro_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "#Define $strName(" );
					$this->_DisplayParams( $arrParams );
					$hHTML->Text( ") " );
					$this->_DisplayCrossedLink( $iBodyXLinkID );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefMacro_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefMacro_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefMacro_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefMacro_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefMacro_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}

	private function _DisplayNamespace( $iNamespaceID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrNamespaceInfos = $this->GetNamespaceInfos( $iNamespaceID );
		$strName = $arrNamespaceInfos["Name"];
		$strFileName = $arrNamespaceInfos["FileName"];
		$iLineNumber = $arrNamespaceInfos["LineNumber"];

		$strProtection = $arrNamespaceInfos["Protection"];

		$strShortDesc = $arrNamespaceInfos["ShortDesc"];
		$strDocContent = $arrNamespaceInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefNamespace" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefNamespace_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "$strProtection namespace $strName" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefNamespace_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefNamespace_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefNamespace_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefNamespace_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefNamespace_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayType( $iTypeID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrTypeInfos = $this->GetTypeInfos( $iTypeID );
		$strName = $arrTypeInfos["Name"];
		$strFileName = $arrTypeInfos["FileName"];
		$iLineNumber = $arrTypeInfos["LineNumber"];

		$arrTemplateParams = $arrTypeInfos["TemplateParams"];
		$strProtection = $arrTypeInfos["Protection"];
		$strMetaType = $arrTypeInfos["MetaType"];

		$strShortDesc = $arrTypeInfos["ShortDesc"];
		$strDocContent = $arrTypeInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefType" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefType_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "$strProtection " );
					if ( count($arrTemplateParams) > 0 ) {
						$hHTML->Text( "template<" );
						$this->_DisplayParams( $arrTemplateParams );
						$hHTML->Text( ">" );
						$hHTML->LineBreak();
					}
					$hHTML->Text( "$strMetaType $strName" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefType_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefType_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefType_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefType_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefType_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}

	private function _DisplayEnum( $iEnumID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrEnumInfos = $this->GetEnumInfos( $iEnumID );
		$strName = $arrEnumInfos["Name"];
		$strFileName = $arrEnumInfos["FileName"];
		$iLineNumber = $arrEnumInfos["LineNumber"];

		$strProtection = $arrEnumInfos["Protection"];
		$arrEnumValues = $arrEnumInfos["EnumValues"];

		$strShortDesc = $arrEnumInfos["ShortDesc"];
		$strDocContent = $arrEnumInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefEnum" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefEnum_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "$strProtection enum $strName" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefEnum_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefEnum_EnumValues" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Enum Values" );
				$hHTML->DivEnd();
				foreach( $arrEnumValues as $strName => $iXLinkID ) {
					$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "$strName" );
						$arrXLinkInfos = $this->GetXLinkInfos( $iXLinkID );
						if ( $arrXLinkInfos["LinkText"] != "" ) {
							$hHTML->Text( " = " );
							$this->_DisplayCrossedLink( $iXLinkID );
						}
					$hHTML->DivEnd();
				}
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefEnum_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefEnum_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefEnum_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefEnum_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayTypedef( $iTypedefID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrTypedefInfos = $this->GetTypedefInfos( $iTypedefID );
		$strName = $arrTypedefInfos["Name"];
		$strFileName = $arrTypedefInfos["FileName"];
		$iLineNumber = $arrTypedefInfos["LineNumber"];

		$strProtection = $arrTypedefInfos["Protection"];
		$iTargetTypeXLinkID = $arrTypedefInfos["TargetTypeLink"];
		$arrParams = $arrTypedefInfos["Params"];

		$strShortDesc = $arrTypedefInfos["ShortDesc"];
		$strDocContent = $arrTypedefInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefTypedef" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefTypedef_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "$strProtection typedef " );
					$this->_DisplayCrossedLink( $iTargetTypeXLinkID );
					if ( $arrParams > 0 ) {
						$hHTML->Text( "(" );
						$this->_DisplayParams( $arrParams );
						$hHTML->Text( ")" );
					}
					$hHTML->Text( " $strName" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefTypedef_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefTypedef_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefTypedef_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefTypedef_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefTypedef_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}

	private function _DisplayVariable( $iVariableID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrVariableInfos = $this->GetVariableInfos( $iVariableID );
		$strName = $arrVariableInfos["Name"];
		$strFileName = $arrVariableInfos["FileName"];
		$iLineNumber = $arrVariableInfos["LineNumber"];

		$strProtection = $arrVariableInfos["Protection"];
		$strSpecifiers = "";
		if ( $arrVariableInfos["Static"] )
			$strSpecifiers .= "static ";
		if ( $arrVariableInfos["Const"] )
			$strSpecifiers .= "const ";
		else if ( $arrVariableInfos["Volatile"] )
			$strSpecifiers .= "volatile ";
		else if ( $arrVariableInfos["Mutable"] )
			$strSpecifiers .= "mutable ";
		$arrTemplateParams = $arrVariableInfos["TemplateParams"];
		$iInstanceTypeXLinkID = $arrVariableInfos["InstanceTypeLink"];
		$iInitializeXLinkID = $arrVariableInfos["InitializeLink"];

		$strShortDesc = $arrVariableInfos["ShortDesc"];
		$strDocContent = $arrVariableInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefVariable" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefVariable_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "$strProtection $strSpecifiers " );
					if ( count($arrTemplateParams) > 0 ) {
						$hHTML->Text( "template<" );
						$this->_DisplayParams( $arrTemplateParams );
						$hHTML->Text( ">" );
						$hHTML->LineBreak();
					}
					$this->_DisplayCrossedLink( $iInstanceTypeXLinkID );
					$hHTML->Text( " $strName" );
					$arrXLinkInfos = $this->GetXLinkInfos( $iInitializeXLinkID );
					if ( $arrXLinkInfos["LinkText"] != "" ) {
						$hHTML->Text( " = " );
						$this->_DisplayCrossedLink( $iInitializeXLinkID );
					}
					$hHTML->Text( ";" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefVariable_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefVariable_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefVariable_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefVariable_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefVariable_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayFunction( $iFunctionID )
	{
		$hHTML = HTMLManager::GetSingleton();

		$arrFunctionInfos = $this->GetFunctionInfos( $iFunctionID );
		$strName = $arrFunctionInfos["Name"];
		$strFileName = $arrFunctionInfos["FileName"];
		$iLineNumber = $arrFunctionInfos["LineNumber"];

		$arrTemplateParams = $arrFunctionInfos["TemplateParams"];
		$strProtection = $arrFunctionInfos["Protection"];
		$strSpecifiers = "";
		if ( $arrFunctionInfos["Inline"] )
			$strSpecifiers .= "inline ";
		if ( $arrFunctionInfos["Static"] )
			$strSpecifiers .= "static ";
		else if ( $arrFunctionInfos["Virtual"] != CODEDOC_XML_VIRTUAL_NO )
			$strSpecifiers .= "virtual ";
		$iReturnTypeXLinkID = $arrFunctionInfos["ReturnTypeLink"];
		$arrParams = $arrFunctionInfos["Params"];

		$strShortDesc = $arrFunctionInfos["ShortDesc"];
		$strDocContent = $arrFunctionInfos["DocContent"];

		$hHTML->SetID( "CodeDocContent_RefFunction" );
		$hHTML->SetStyleClass( "CodeDocContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "CodeDocContent_RefFunction_Declaration" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Declaration" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "$strProtection $strSpecifiers " );
					if ( count($arrTemplateParams) > 0 ) {
						$hHTML->Text( "template<" );
						$this->_DisplayParams( $arrTemplateParams );
						$hHTML->Text( ">" );
						$hHTML->LineBreak();
					}
					$this->_DisplayCrossedLink( $iReturnTypeXLinkID );
					$hHTML->Text( " $strName(" );
					$this->_DisplayParams( $arrParams );
					$hHTML->Text( ")" );
					if ( $arrFunctionInfos["Const"] )
						$hHTML->Text( " const" );
					if ( $arrFunctionInfos["Virtual"] == CODEDOC_XML_VIRTUAL_PURE )
						$hHTML->Text( " = 0" );
					$hHTML->Text( ";" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "CodeDocContent_RefFunction_Location" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Location" );
				$hHTML->DivEnd();
				$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( $strFileName );
					$hHTML->LineBreak();
					$hHTML->Text( "At line $iLineNumber" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$iParamCount = count( $arrParams );
			if ( $iParamCount > 0 ) {
				$hHTML->SetID( "CodeDocContent_RefFunction_ArgumentsHeader" );
				$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Arguments" );
				$hHTML->DivEnd();

				for( $i = 0; $i < $iParamCount; ++$i ) {
					$hHTML->SetID( "CodeDocContent_RefFunction_Argument_$i" );
					$hHTML->SetStyleClass( "CodeDocContainer_Level2C ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_SubHeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( $arrParams[$i][0] );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CodeDocContainer_Level3C CodeDocProperties_Item_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Coming soon ..." );
							// Extract this from $strDocContent
						$hHTML->DivEnd();
					$hHTML->DivEnd();
				}
			}

			$arrXLinkInfos = $this->GetXLinkInfos( $iReturnTypeXLinkID );
			if ( $arrXLinkInfos["LinkText"] != "Void" ) {
				$hHTML->SetID( "CodeDocContent_RefFunction_ReturnValueHeader" );
				$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Return Value" );
				$hHTML->DivEnd();
				$hHTML->SetID( "CodeDocContent_RefFunction_ReturnValue" );
				$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->Text( "Coming soon ..." );
					// Extract this from $strDocContent
				$hHTML->DivEnd();
			}

			$hHTML->SetID( "CodeDocContent_RefFunction_ShortDescHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Overview" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefFunction_ShortDesc" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strShortDesc );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "CodeDocContent_RefFunction_DocContentHeader" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_HeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Description" );
			$hHTML->DivEnd();
			$hHTML->SetID( "CodeDocContent_RefFunction_DocContent" );
			$hHTML->SetStyleClass( "CodeDocContainer_Level2C CodeDocProperties_ContentText" );
			$hHTML->DivStart();
				$hHTML->Text( $strDocContent );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}

	// Members
}
?>