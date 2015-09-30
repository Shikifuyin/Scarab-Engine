<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/SQL/SQL.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SQL queries interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
require_once "_DBConnection.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
define( "SQLQUERY_FETCH_ROW", 0 );
define( "SQLQUERY_FETCH_MAP", 1 );
define( "SQLQUERY_FETCH_OBJ", 2 );

/////////////////////////////////////////////////////////////////////////////////
// The SQLManager class
class SQLManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( SQLManager::$m_hInstance == NULL )
            SQLManager::$m_hInstance = new SQLManager();
        return SQLManager::$m_hInstance;
    }
    protected static $m_hInstance;

    // Constructor / Destructor
    public function __construct()
    {
        // Disconnected state
        $this->m_hSQL = NULL;
        $this->m_hQuery = NULL;
		
		$this->m_bUseTempConnection = false;
    }
	public function __destruct()
    {
		// nothing to do
	}
    
    // Database connection
	public function IsConnected() {
		return ( $this->m_hSQL != NULL );
	}
	public function ConnectTemporary() {
		if ( $this->IsConnected() )
			return;
		$bOK = $this->Connect( SQLCONNECTION_HOSTNAME, SQLCONNECTION_LOGIN, SQLCONNECTION_PASSWORD, SQLCONNECTION_DBNAME );
		assert( '$bOK' );
		$this->m_bUseTempConnection = true;
	}
	public function DisconnectTemporary() {
		if ( !($this->m_bUseTempConnection) );
			return;
		$bOK = $this->Disconnect();
		assert( '$bOK' );
		$this->m_bUseTempConnection = false;
	}

    public function Connect( $strHost, $strUser, $strPassword, $strDatabase )
    {
        assert( '$this->m_hSQL == NULL' );
        assert( '$this->m_hQuery == NULL' );

        $this->m_hSQL = new mysqli( $strHost, $strUser, $strPassword, $strDatabase );
        if ( $this->m_hSQL->connect_errno ) {
            echo "Database : Connection failed : (" . $this->m_hSQL->connect_errno . ") " . $this->m_hSQL->connect_error;
            $this->m_hSQL = NULL;
            return false;
        }

        return true;
    }
    public function Disconnect()
    {
        assert( '$this->m_hSQL != NULL' );

        if ( $this->m_hQuery != NULL ) {
            if ( $this->m_hQuery instanceof mysqli_result )
            	$this->m_hQuery->free();
            $this->m_hQuery = NULL;
        }

        $bOK = $this->m_hSQL->close();
        if ( !$bOK ) {
            echo "Database : Disconnection failed : (" . $this->m_hSQL->connect_errno . ") " . $this->m_hSQL->connect_error;
            return false;
        }
        $this->m_hSQL = NULL;

        return true;
    }
    public function SwitchUser( $strUser, $strPassword, $strDatabase )
    {
        assert( '$this->m_hSQL != NULL' );

        if ( $this->m_hQuery != NULL ) {
            if ( $this->m_hQuery instanceof mysqli_result )
            	$this->m_hQuery->free();
            $this->m_hQuery = NULL;
        }

        $bOK = $this->m_hSQL->change_user( $strUser, $strPassword, $strDatabase );
        if ( !$bOK ) {
            echo "Database : Switching user failed : (" . $this->m_hSQL->connect_errno . ") " . $this->m_hSQL->connect_error;
            return false;
        }

        return true;
    }
    public function SelectDB( $strDatabase )
    {
        assert( '$this->m_hSQL != NULL' );

        if ( $this->m_hQuery != NULL ) {
            if ( $this->m_hQuery instanceof mysqli_result )
            	$this->m_hQuery->free();
            $this->m_hQuery = NULL;
        }

        $bOK = $this->m_hSQL->select_db( $strDatabase );
        if ( !$bOK ) {
            echo "Database : Selecting database failed : (" . $this->m_hSQL->connect_errno . ") " . $this->m_hSQL->connect_error;
            return false;
        }

        return true;
    }

    // Database management (auto-commit is ON by default)
    public function Commit()
    {
        assert( '$this->m_hSQL != NULL' );

        if ( $this->m_hQuery != NULL ) {
            if ( $this->m_hQuery instanceof mysqli_result )
            	$this->m_hQuery->free();
            $this->m_hQuery = NULL;
        }

        $bOK = $this->m_hSQL->commit();
        if ( !$bOK ) {
            echo "Database : Commit failed : (" . $this->m_hSQL->errno . ") " . $this->m_hSQL->error;
            return false;
        }
        
        return true;
    }
    public function Rollback()
    {
        assert( '$this->m_hSQL != NULL' );

        if ( $this->m_hQuery != NULL ) {
            if ( $this->m_hQuery instanceof mysqli_result )
            	$this->m_hQuery->free();
            $this->m_hQuery = NULL;
        }

        $bOK = $this->m_hSQL->rollback();
        if ( !$bOK ) {
            echo "Database : Rollback failed : (" . $this->m_hSQL->errno . ") " . $this->m_hSQL->error;
            return false;
        }
        
        return true;
    }

    public function SetAutoCommit( $bAutoCommit = true )
    {
        assert( '$this->m_hSQL != NULL' );
        
        $bOK = $this->m_hSQL->autocommit( $bAutoCommit );
        if ( !$bOK ) {
            echo "Database : Setting autocommit failed : (" . $this->m_hSQL->errno . ") " . $this->m_hSQL->error;
            return false;
        }
        
        return true;
    }

    // Query interface
    public function Query( $strQuery ) {
        assert( '$this->m_hSQL != NULL' );
		$arrArgs = func_get_args();
		array_shift( $arrArgs );
		return $this->_BaseQuery( $strQuery, $arrArgs );
    }
    public function FetchRow( $iFetchType = SQLQUERY_FETCH_MAP )
    {
        assert( '$this->m_hSQL != NULL' );
        assert( '$this->m_hQuery != NULL' );

        switch( $iFetchType ) {
            case SQLQUERY_FETCH_ROW: return $this->m_hQuery->fetch_row();
            case SQLQUERY_FETCH_MAP: return $this->m_hQuery->fetch_assoc();
            case SQLQUERY_FETCH_OBJ: {
					$objRow = $this->m_hQuery->fetch_object();
					// Enumerate members ?
					assert( 'false' );
					return $objRow;
				}
            default: assert('false'); return NULL;
        }
    }
	public function FreeResults() {
		assert( '$this->m_hSQL != NULL' );

        if ( $this->m_hQuery != NULL ) {
            if ( $this->m_hQuery instanceof mysqli_result )
            	$this->m_hQuery->free();
            $this->m_hQuery = NULL;
        }
	}

    public function GetRowCount() {
        assert( '$this->m_hSQL != NULL' );
        assert( '$this->m_hQuery != NULL' );
        return $this->m_hQuery->num_rows;
    }
    public function GetColumnCount() {
        assert( '$this->m_hSQL != NULL' );
        assert( '$this->m_hQuery != NULL' );
        return $this->m_hQuery->field_count;
    }

    public function GetAffectedRows() {
        assert( '$this->m_hSQL != NULL' );
        return $this->m_hSQL->affected_rows;
    }
    public function GetAutoIncrement() {
        assert( '$this->m_hSQL != NULL' );
        return $this->m_hSQL->insert_id;
    }

	// Specialized queries interface
	public function Select( $strTable, $arrKeys = NULL, $strCondition = NULL, $arrConditionValues = NULL, $strOrdering = NULL )
	{
        assert( '$this->m_hSQL != NULL' );

		$strKeys = "*";
		if ( $arrKeys != NULL )
			$strKeys = implode( ", ", $arrKeys );
		$arrParams = array();

		$strQuery = "SELECT $strKeys FROM $strTable";
		if ( $strCondition != NULL ) {
			$strQuery .= " WHERE $strCondition";
			if ( $arrConditionValues != NULL ) {
				foreach( $arrConditionValues as $mValue )
					array_push( $arrParams, "$mValue" );
			}
		}
		if ( $strOrdering != NULL )
			$strQuery .= " ORDER BY $strOrdering";

		$bOK = $this->_BaseQuery( $strQuery, $arrParams );
		assert( '$bOK' );
	}
	public function Insert( $strTable, $arrRow )
	{
        assert( '$this->m_hSQL != NULL' );

		$strKeys = "";
		$strValues = "";
		$arrParams = array();
		$bNonEmpty = false;
		foreach( $arrRow as $strKey => $mValue ) {
			$strKeys .= "$strKey, ";
			$strValues .= ( is_int($mValue) ) ? "?, " : "'?', ";
			array_push( $arrParams, "$mValue" );
			$bNonEmpty = true;
		}
		if ( $bNonEmpty ) {
			$strKeys = substr( $strKeys, 0, -2 );
			$strValues = substr( $strValues, 0, -2 );
		}
		
		$strQuery = "INSERT INTO $strTable ( $strKeys ) VALUES ( $strValues )";

		$bOK = $this->_BaseQuery( $strQuery, $arrParams );
		if ( !$bOK ) {
			print_r( "<br/> Insert row = " );
			print_r( $arrRow );
			print_r( "<br/> Query string = $strQuery" );
			print_r( "<br/> Params = " );
			print_r( $arrParams );
		}
		assert( '$bOK' );
	}
	public function Update( $strTable, $arrRow, $strCondition = NULL, $arrConditionValues = NULL )
	{
        assert( '$this->m_hSQL != NULL' );

		$strSets = "";
		$arrParams = array();
		$bNonEmpty = false;
		foreach( $arrRow as $strKey => $mValue ) {
			$strSets .= ( is_int($mValue) ) ? "$strKey=?, " : "$strKey='?', ";
			array_push( $arrParams, "$mValue" );
			$bNonEmpty = true;
		}
		if ( $bNonEmpty )
			$strSets = substr( $strSets, 0, -2 );

		$strQuery = "UPDATE $strTable SET $strSets";
		if ( $strCondition != NULL ) {
			$strQuery .= " WHERE $strCondition";
			if ( $arrConditionValues != NULL ) {
				foreach( $arrConditionValues as $mValue )
					array_push( $arrParams, "$mValue" );
			}
		}

		$bOK = $this->_BaseQuery( $strQuery, $arrParams );
		assert( '$bOK' );
	}
	public function Delete( $strTable, $strCondition = NULL, $arrConditionValues = NULL )
	{
        assert( '$this->m_hSQL != NULL' );

		$arrParams = array();

		$strQuery = "DELETE FROM $strTable";
		if ( $strCondition != NULL ) {
			$strQuery .= " WHERE $strCondition";
			if ( $arrConditionValues != NULL ) {
				foreach( $arrConditionValues as $mValue )
					array_push( $arrParams, "$mValue" );
			}
		}

		$bOK = $this->_BaseQuery( $strQuery, $arrParams );
		assert( '$bOK' );
	}

	public function CreateTable()
	{
        assert( '$this->m_hSQL != NULL' );

		/////////////////////////////////////////////////
	}
	public function DropTable( $strTable )
	{
        assert( '$this->m_hSQL != NULL' );

		$strQuery = "DROP TABLE IF EXISTS $strTable";

		$bOK = $this->_BaseQuery( $strQuery, array() );
		assert( '$bOK' );
	}

	// Helpers
	public function _BaseQuery( $strQuery, $arrParams )
    {
        if ( $this->m_hQuery != NULL ) {
			if ( $this->m_hQuery instanceof mysqli_result )
            	$this->m_hQuery->free();
            $this->m_hQuery = NULL;
        }

        $iParamCount = count( $arrParams );
        if ( $iParamCount > 0 ) {
			$bLikeArg = false;
			$i = 0;
            while( $i < $iParamCount ) {
				if ( $arrParams[$i] == "LARG" ) {
					array_splice( $arrParams, $i, 1 );
					$bLikeArg = true;
					--$iParamCount;
					continue;
				}
				if ( $bLikeArg ) {
					$arrParams[$i] = addcslashes( $arrParams[$i], "%_" );
					$bLikeArg = false;
				}
                $arrParams[$i] = $this->m_hSQL->real_escape_string( $arrParams[$i] );
				++$i;
            }
            $strQuery = str_replace( "%", "%%", $strQuery );
            $strQuery = str_replace( "?", "%s", $strQuery );
            $strQuery = vsprintf( $strQuery, $arrParams );
        }

        $this->m_hQuery = $this->m_hSQL->query( $strQuery );
        if ( $this->m_hQuery == NULL ) {
            echo "Database : Executing query failed : (" . $this->m_hSQL->errno . ") " . $this->m_hSQL->error;
            return false;
        }

        return true;
    }


    // Members
    private $m_hSQL;
    private $m_hQuery;

	private $m_bUseTempConnection;
}

?>