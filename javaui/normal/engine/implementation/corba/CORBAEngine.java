
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Java user interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

package normal.engine.implementation.corba;

import java.applet.Applet;
import java.util.Properties;
import normal.Shell;
import normal.engine.*;
import normal.engine.algebra.*;
import normal.engine.file.*;
import normal.engine.maths.*;
import normal.engine.packet.*;
import normal.engine.progress.*;
import normal.engine.subcomplex.*;
import normal.engine.surfaces.*;
import normal.engine.triangulation.*;
import normal.engine.utilities.*;
import normal.engine.implementation.corba.algebra.*;
import normal.engine.implementation.corba.file.*;
import normal.engine.implementation.corba.maths.*;
import normal.engine.implementation.corba.packet.*;
import normal.engine.implementation.corba.progress.*;
import normal.engine.implementation.corba.subcomplex.*;
import normal.engine.implementation.corba.surfaces.*;
import normal.engine.implementation.corba.triangulation.*;
import org.omg.CosNaming.*;
import org.omg.CORBA.*;

/**
 * Used when the engine is in a server application to be accessed through
 * CORBA.
 * See <tt>Engine</tt> for more details.
 *
 * @see normal.engine.Engine
 */
public class CORBAEngine implements Engine {
    /**
     * The underlying CORBA object.
     */
    private normal.engine.implementation.corba.Regina.Engine data;
    /**
     * The CORBA name service host being used.
     */
    private String host;
    /**
     * The CORBA name service port being used.
     */
    private String port;

    /**
     * Create a link to the calculation engine using CORBA.
     * This routine should be called only when the program is run as a
     * standalone application.
     * The engine should be in a server application accessible
     * through CORBA via a name service.
     *
     * @param args a list of command-line arguments to the application,
     * possibly including flags that should be passed to the ORB
     * initialisation.
     * @param host the CORBA name service host being requested.  This
     * information is <b>not</b> passed to the ORB initialisation; it
     * is for diagnostic purposes only.  This information needs to be
     * replicated appropriately in parameter <i>args</i>.
     * @param port the CORBA name service port being requested.  This
     * information is <b>not</b> passed to the ORB initialisation; it
     * is for diagnostic purposes only.  This information needs to be
     * replicated appropriately in parameter <i>args</i>.
     * @throws CORBAException thrown when the CORBA connection could not
     * be initialised.
     */
    public CORBAEngine(String[] args, String host, String port)
            throws CORBAException {
        this.host = host;
        this.port = port;
        data = null;
        try {
            ORB orb;
            try {
                orb = ORB.init(args, null);
            } catch (Throwable th) {
                throw new normal.engine.implementation.corba.CORBAException(
                    "The ORB could not be initialised.");
            }
            initEngine(orb);
        } catch (Throwable th) {
            if (th instanceof
                    normal.engine.implementation.corba.CORBAException)
                throw (normal.engine.implementation.corba.CORBAException)th;

            System.out.println("Could not initiate CORBA connection: " +
                th.toString());
            th.printStackTrace();
            throw new normal.engine.implementation.corba.CORBAException();
        }
    }

    /**
     * Create a link to the calculation engine using CORBA.
     * This routine should be called only when the program is run as an
     * applet on a web page.
     * The engine should be in a server application accessible
     * through CORBA via a name service.
     *
     * @param applet the applet we are running, whose parameters might
     * contain flags that should be passed to the ORB initialisation.
     * @param host the CORBA name service host being requested.  This
     * information <b>is</b> passed to the ORB initialisation, but will
     * be overridden by any applet parameters.
     * @param port the CORBA name service port being requested.  This
     * information <b>is</b> passed to the ORB initialisation, but will
     * be overridden by any applet parameters.
     * @throws CORBAException thrown when the CORBA connection could not
     * be initialised.
     */
    public CORBAEngine(Applet applet, String host, String port)
            throws CORBAException {
        this.host = host;
        this.port = port;
        data = null;
        try {
            ORB orb;
            try {
                Properties prop = new Properties();
                prop.setProperty("org.omg.CORBA.ORBInitialHost", host);
                prop.setProperty("org.omg.CORBA.ORBInitialPort", port);
                orb = ORB.init(applet, prop);
            } catch (Throwable th) {
                throw new normal.engine.implementation.corba.CORBAException(
                    "The ORB could not be initialised.");
            }
            initEngine(orb);
        } catch (Throwable th) {
            if (th instanceof
                    normal.engine.implementation.corba.CORBAException)
                throw (normal.engine.implementation.corba.CORBAException)th;

            System.out.println("Could not initiate CORBA connection: " +
                th.toString());
            th.printStackTrace();
            throw new normal.engine.implementation.corba.CORBAException();
        }
    }

    /**
     * Initialises the link to the CORBA calculation engine.
     *
     * @param orb the ORB to use; it is assumed that this has already been
     * initialised.
     * @throws CORBAException thrown when the CORBA connection could not
     * be initialised.
     */
    private void initEngine(ORB orb) throws CORBAException {
        org.omg.CORBA.Object objRef;
        try {
            objRef = orb.resolve_initial_references("NameService");
        } catch (Throwable th) {
            throw new normal.engine.implementation.corba.CORBAException(
                "The requested name service could not be found.");
        }

        NamingContext ncRef;
        try {
            ncRef = NamingContextHelper.narrow(objRef);
        } catch (Throwable th) {
            throw new normal.engine.implementation.corba.CORBAException(
                "The name service could not be narrowed to " +
                "the correct class.");
        }

        NameComponent path[] = {
            new NameComponent("regina", "context"),
            new NameComponent("Engine", "object")
        };
        try {
            objRef = ncRef.resolve(path);
            if (objRef == null)
                throw new Exception();
        } catch (Throwable th) {
            throw new normal.engine.implementation.corba.CORBAException(
                "The CORBA calculation engine could not be located " +
                "by the name service.");
        }

        try {
            data = normal.engine.implementation.corba.Regina.EngineHelper.
                narrow(objRef);
            if (data == null)
                throw new Exception();
        } catch (Throwable th) {
            throw new normal.engine.implementation.corba.CORBAException(
                "The CORBA calculation engine could not be narrowed to " +
                "the correct class.");
        }
    }

    /**
     * Provides a polite string representation for this object.
     * 
     * @return a string representation of this engine.
     */
    public String toString() {
        return normal.Application.program +
            " calculation engine accessed through CORBA";
    }

    public int style() {
        return Shell.engineCORBA;
    }
    public String styleDescription() {
        return "accessed through CORBA (" + host + ":" + port + ")";
    }

    public NAbelianGroup newNAbelianGroup() {
        return NCORBAAbelianGroup.newWrapper(data.newNAbelianGroup_());
    }
    public NAbelianGroup newNAbelianGroup(NAbelianGroup cloneMe) {
        return NCORBAAbelianGroup.newWrapper(
            data.newNAbelianGroup_NAbelianGroup(
            ((NCORBAAbelianGroup)cloneMe).data));
    }
    public NContainer newNContainer() {
        return NCORBAContainer.newWrapper(data.newNContainer());
    }
    public NFile newNFile() {
        return NCORBAFile.newWrapper(data.newNFile());
    }
	public NGroupExpression newNGroupExpression() {
		return NCORBAGroupExpression.newWrapper(data.newNGroupExpression_());
	}
	public NGroupExpression newNGroupExpression(NGroupExpression cloneMe) {
		return NCORBAGroupExpression.newWrapper(
			data.newNGroupExpression_NGroupExpression(
			((NCORBAGroupExpression)cloneMe).data));
	}
	public NGroupPresentation newNGroupPresentation() {
		return NCORBAGroupPresentation.newWrapper(
			data.newNGroupPresentation_());
	}
	public NGroupPresentation newNGroupPresentation(
			NGroupPresentation cloneMe) {
		return NCORBAGroupPresentation.newWrapper(
			data.newNGroupPresentation_NGroupPresentation(
			((NCORBAGroupPresentation)cloneMe).data));
	}
    public NMatrixInt newNMatrixInt(int rows, int columns) {
        return NCORBAMatrixInt.newWrapper(data.newNMatrixInt_long_long(
            rows, columns));
    }
    public NMatrixInt newNMatrixInt(NMatrixInt cloneMe) {
        return NCORBAMatrixInt.newWrapper(data.newNMatrixInt_NMatrixInt(
            ((NCORBAMatrixInt)cloneMe).data));
    }
    public NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner,
            int flavour) {
        return newNNormalSurfaceList(owner, flavour, true);
    }
    public NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner,
            int flavour, boolean isEmbeddedOnly) {
        return NCORBANormalSurfaceList.newWrapper(data.newNNormalSurfaceList(
            ((NCORBATriangulation)owner).data, flavour, isEmbeddedOnly));
    }
    public NProgressManager newNProgressManager() {
        return NCORBAProgressManager.newWrapper(data.newNProgressManager());
    }
    public NScript newNScript() {
        return NCORBAScript.newWrapper(data.newNScript());
    }
    public NSurfaceFilter newNSurfaceFilter() {
        return NCORBASurfaceFilter.newWrapper(data.newNSurfaceFilter_());
    }
    public NSurfaceFilter newNSurfaceFilter(NSurfaceFilter cloneMe) {
        return NCORBASurfaceFilter.newWrapper(
            data.newNSurfaceFilter_NSurfaceFilter(
            ((NCORBASurfaceFilter)cloneMe).data));
    }
    public NSurfaceFilterCombination newNSurfaceFilterCombination() {
        return NCORBASurfaceFilterCombination.newWrapper(
            data.newNSurfaceFilterCombination_());
    }
    public NSurfaceFilterCombination newNSurfaceFilterCombination(
            NSurfaceFilterCombination cloneMe) {
        return NCORBASurfaceFilterCombination.newWrapper(
            data.newNSurfaceFilterCombination_NSurfaceFilterCombination(
            ((NCORBASurfaceFilterCombination)cloneMe).data));
    }
    public NSurfaceFilterProperties newNSurfaceFilterProperties() {
        return NCORBASurfaceFilterProperties.newWrapper(
            data.newNSurfaceFilterProperties_());
    }
    public NSurfaceFilterProperties newNSurfaceFilterProperties(
            NSurfaceFilterProperties cloneMe) {
        return NCORBASurfaceFilterProperties.newWrapper(
            data.newNSurfaceFilterProperties_NSurfaceFilterProperties(
            ((NCORBASurfaceFilterProperties)cloneMe).data));
    }
    public NSurfaceSubset newNSurfaceSubset(NSurfaceSet set,
            NSurfaceFilter filter) {
        return NCORBASurfaceSubset.newWrapper(data.newNSurfaceSubset(
            ((NCORBASurfaceSet)set).getNSurfaceSet(),
            ((NCORBASurfaceFilter)filter).data));
    }
    public NTetrahedron newNTetrahedron() {
        return NCORBATetrahedron.newWrapper(data.newNTetrahedron_());
    }
    public NTetrahedron newNTetrahedron(String desc) {
        return NCORBATetrahedron.newWrapper(data.newNTetrahedron_string(desc));
    }
    public NText newNText() {
        return NCORBAText.newWrapper(data.newNText_());
    }
    public NText newNText(String text) {
        return NCORBAText.newWrapper(data.newNText_string(text));
    }
    public NTriangulation newNTriangulation() {
        return NCORBATriangulation.newWrapper(data.newNTriangulation_());
    }
    public NTriangulation newNTriangulation(NTriangulation cloneMe) {
        return NCORBATriangulation.newWrapper(
            data.newNTriangulation_NTriangulation(
            ((NCORBATriangulation)cloneMe).data));
    }

    public NTriangulation enterTextTriangulation() {
        return null;
    }
    public long formCensus(NPacket parent, int nTetrahedra,
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary) {
        return formCensus(parent, nTetrahedra, finiteness,
            orientability, boundary, -1, null);
    }
    public long formCensus(NPacket parent, int nTetrahedra,
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
            int nBdryFaces) {
        return formCensus(parent, nTetrahedra, finiteness,
            orientability, boundary, nBdryFaces, null);
    }
    public long formCensus(NPacket parent, int nTetrahedra, 
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
            int nBdryFaces, NProgressManager manager) {
        return data.formCensus(((NCORBAPacket)parent).data, nTetrahedra,
            finiteness.getByteCode(), orientability.getByteCode(),
            boundary.getByteCode(), nBdryFaces,
            ((NCORBAProgressManager)manager).data);
    }
	public NSnappedTwoSphere formsSnappedTwoSphere(
			NSnappedBall ball1, NSnappedBall ball2) {
		return NCORBASnappedTwoSphere.newWrapper(
			data.formsSnappedTwoSphere_NSnappedBall(
			((NCORBASnappedBall)ball1).data, ((NCORBASnappedBall)ball2).data));
	}
	public NSnappedTwoSphere formsSnappedTwoSphere(
			NTetrahedron tet1, NTetrahedron tet2) {
		return NCORBASnappedTwoSphere.newWrapper(
			data.formsSnappedTwoSphere_NTetrahedron(
			((NCORBATetrahedron)tet1).data, ((NCORBATetrahedron)tet2).data));
	}
    public int getVersionMajor() {
        return data.getVersionMajor();
    }
    public int getVersionMinor() {
        return data.getVersionMinor();
    }
    public String getVersionString() {
        return data.getVersionString();
    }
	public NLayeredLensSpace isLayeredLensSpace(NComponent comp) {
		return NCORBALayeredLensSpace.newWrapper(
			data.isLayeredLensSpace(((NCORBAComponent)comp).data));
	}
	public NLayeredSolidTorus isLayeredSolidTorusBase(NTetrahedron tet) {
		return NCORBALayeredSolidTorus.newWrapper(
			data.isLayeredSolidTorusBase(((NCORBATetrahedron)tet).data));
	}
	public NSnappedBall isSnappedBall(NTetrahedron tet) {
		return NCORBASnappedBall.newWrapper(
			data.isSnappedBall(((NCORBATetrahedron)tet).data));
	}
    public NMatrixInt makeMatchingEquations(NTriangulation
            triangulation, int flavour) {
        return NCORBAMatrixInt.newWrapper(data.makeMatchingEquations(
            ((NCORBATriangulation)triangulation).data, flavour));
    }
    public NPacket readFromFile(String fileName) {
        return NCORBAPacket.newWrapper(data.readFromFile(fileName));
    }
    public NTriangulation readSnapPea(String file) {
        return NCORBATriangulation.newWrapper(data.readSnapPea(file));
    }
    public void smithNormalForm(NMatrixInt matrix) {
        data.smithNormalForm(((NCORBAMatrixInt)matrix).data);
    }
    public int testEngine(int value) {
        return data.testEngine(value);
    }
    public boolean writeToFile(String fileName, NPacket packet) {
        return data.writeToFile(fileName, ((NCORBAPacket)packet).data);
    }
}
