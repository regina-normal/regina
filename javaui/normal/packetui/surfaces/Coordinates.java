
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

package normal.packetui.surfaces;

import normal.engine.surfaces.*;
import normal.engine.triangulation.NTriangulation;
import java.math.BigInteger;
import java.util.Vector;

/**
 * Contains routines to handle the variety of available coordinate
 * systems.  Whenever a new flavour of coordinate system is introduced,
 * only this class should need to be edited.
 */
public class Coordinates {
    /**
     * Returns a list of all flavours of coordinate system available for
     * creating normal surface lists.  Each object in the returned
     * vector will be of type <tt>Coordinates</tt> and will be suitable
     * for placing in a combo box or the like.
     *
     * @return a list of coordinate systems.
     */
    public static Vector allCreators() {
        Vector ans = new Vector();
        ans.addElement(new Coordinates(NNormalSurfaceList.STANDARD));
        ans.addElement(new Coordinates(NNormalSurfaceList.AN_STANDARD));
        ans.addElement(new Coordinates(NNormalSurfaceList.QUAD));
        return ans;
    }

    /**
     * Returns a list of all flavours of coordinate system capable of
     * viewing the given normal surface set.  Each object in the returned
     * vector will be of type <tt>Coordinates</tt> and will be suitable
     * for placing in a combo box or the like.
     * <p>
     * If the given set is <tt>null</tt>, an empty vector will be
     * returned.
     *
     * @param set the normal surface set to be viewed.
     * @return a list of coordinate systems.
     */
    public static Vector allViewers(NSurfaceSet set) {
        if (set == null)
            return new Vector();

        boolean almostNormal = set.allowsAlmostNormal();
        Vector ans = new Vector();
        if (almostNormal) {
            ans.addElement(new Coordinates(NNormalSurfaceList.AN_STANDARD));
        } else {
            ans.addElement(new Coordinates(NNormalSurfaceList.STANDARD));
            ans.addElement(new Coordinates(NNormalSurfaceList.QUAD));
        }
        ans.addElement(new Coordinates(NNormalSurfaceList.EDGE_WEIGHT));
        ans.addElement(new Coordinates(NNormalSurfaceList.FACE_ARCS));
        return ans;
    }

    /**
     * Returns the name of the given flavour of coordinate system.
     * The name will begin with a capital and <i>not</i> end with the word
     * <i>coordinates</i> or a period.
     *
     * @param flavour the flavour of coordinate system to use; this must
     * be one of the coordinate system constants defined in
     * <tt>NNormalSurfaceList</tt>.
     * @return the name of the given flavour of coordinate system.
     * @see normal.engine.surfaces.NNormalSurfaceList
     */
    public static String coordinateSystemName(int flavour) {
        switch(flavour) {
            case NNormalSurfaceList.STANDARD:
                return "Standard normal (tri-quad)";
            case NNormalSurfaceList.AN_STANDARD:
                return "Standard almost normal (tri-quad-oct)";

            case NNormalSurfaceList.QUAD:
                return "Quad normal";

            case NNormalSurfaceList.EDGE_WEIGHT:
                return "Edge weight";
            case NNormalSurfaceList.FACE_ARCS:
                return "Face arcs";
        }
        return null;
    }

    /**
     * Returns an abbreviation for the given coordinate in the given
     * flavour of coordinate system.
     *
     * @param flavour the flavour of coordinate system to use; this must
     * be one of the coordinate system constants defined in
     * <tt>NNormalSurfaceList</tt>.
     * @param triang the triangulation in which the normal surface is to
     * reside.
     * @param coord the coordinate to examine; this should be between 0
     * and one less than the total number of coordinates (inclusive).
     * @return an abbreviation for the given coordinate.
     * @see normal.engine.surfaces.NNormalSurfaceList
     */
    public static String getCoordinateAbbr(int flavour,
            NTriangulation triang, int coord) {
        String ans;
        int tmp;
        switch(flavour) {
            case NNormalSurfaceList.STANDARD:
                tmp = coord % 7;
                ans = String.valueOf(coord / 7) + ": ";
                if (tmp < 4)
                    return ans + String.valueOf(tmp);
                else
                    return ans + NNormalSurface.vertexSplitString[tmp - 4];
            case NNormalSurfaceList.AN_STANDARD:
                tmp = coord % 10;
                ans = String.valueOf(coord / 10) + ": ";
                if (tmp < 4)
                    return "T" + ans + String.valueOf(tmp);
                else if (tmp < 7)
                    return "Q" + ans +
                        NNormalSurface.vertexSplitString[tmp - 4];
                else
                    return "K" + ans +
                        NNormalSurface.vertexSplitString[tmp - 7];
            case NNormalSurfaceList.QUAD:
                return String.valueOf(coord / 3) + ": " +
                    NNormalSurface.vertexSplitString[coord % 3];
            case NNormalSurfaceList.EDGE_WEIGHT:
                if (triang.getEdge(coord).isBoundary())
                    return String.valueOf(coord) + " [B]";
                else
                    return String.valueOf(coord);
            case NNormalSurfaceList.FACE_ARCS:
                return String.valueOf(coord / 3) + ": " +
                    String.valueOf(coord % 3);
        }
        return null;
    }

    /**
     * Returns a description of the given coordinate in the given
     * flavour of coordinate system.
     *
     * @param flavour the flavour of coordinate system to use; this must
     * be one of the coordinate system constants defined in
     * <tt>NNormalSurfaceList</tt>.
     * @param triang the triangulation in which the normal surface is to
     * reside.
     * @param coord the coordinate to examine; this should be between 0
     * and one less than the total number of coordinates (inclusive).
     * @return a description of the given coordinate.
     * @see normal.engine.surfaces.NNormalSurfaceList
     */
    public static String getCoordinateDesc(int flavour,
            NTriangulation triang, int coord) {
        String ans;
        int tmp;
        switch(flavour) {
            case NNormalSurfaceList.STANDARD:
                tmp = coord % 7;
                ans = "Tetrahedron " + String.valueOf(coord / 7) + ", ";
                if (tmp < 4)
                    return ans + "triangle about vertex " + String.valueOf(tmp);
                else
                    return ans + "quad splitting vertices " +
                        NNormalSurface.vertexSplitString[tmp - 4];
            case NNormalSurfaceList.AN_STANDARD:
                tmp = coord % 10;
                ans = "Tetrahedron " + String.valueOf(coord / 10) + ", ";
                if (tmp < 4)
                    return ans + "triangle about vertex " + String.valueOf(tmp);
                else if (tmp < 7)
                    return ans + "quad splitting vertices " +
                        NNormalSurface.vertexSplitString[tmp - 4];
                else
                    return ans + "oct partitioning vertices " +
                        NNormalSurface.vertexSplitString[tmp - 7];
            case NNormalSurfaceList.QUAD:
                return "Tetrahedron " + String.valueOf(coord / 3) +
                    ", quad splitting vertices " +
                    NNormalSurface.vertexSplitString[coord % 3];
            case NNormalSurfaceList.EDGE_WEIGHT:
                if (triang.getEdge(coord).isBoundary())
                    return "Weight of (boundary) edge " + String.valueOf(coord);
                else
                    return "Weight of (internal) edge " + String.valueOf(coord);
            case NNormalSurfaceList.FACE_ARCS:
                return "Arcs on face " + String.valueOf(coord / 3) +
                    " crossing face vertex " + String.valueOf(coord % 3);
        }
        return null;
    }

    /**
     * Returns the number of coordinates required for a normal surface
     * in the given triangulation using the given flavour of coordinate
     * system.
     *
     * @param flavour the flavour of coordinate system to use; this must
     * be one of the coordinate system constants defined in
     * <tt>NNormalSurfaceList</tt>.
     * @param triang the triangulation in which the normal surface is to
     * reside.
     * @return the number of coordinate required.
     * @see normal.engine.surfaces.NNormalSurfaceList
     */
    public static int getNumberOfCoordinates(int flavour,
            NTriangulation triang) {
        switch(flavour) {
            case NNormalSurfaceList.STANDARD:
                return 7 * (int)triang.getNumberOfTetrahedra();
            case NNormalSurfaceList.AN_STANDARD:
                return 10 * (int)triang.getNumberOfTetrahedra();
            case NNormalSurfaceList.QUAD:
                return 3 * (int)triang.getNumberOfTetrahedra();
            case NNormalSurfaceList.EDGE_WEIGHT:
                return (int)triang.getNumberOfEdges();
            case NNormalSurfaceList.FACE_ARCS:
                return 3 * (int)triang.getNumberOfFaces();
        }
        return 0;
    }

    /**
     * Returns the requested coordinate for the given normal surface in
     * the given flavour of coordinate system.
     *
     * @param flavour the flavour of coordinate system to use; this must
     * be one of the coordinate system constants defined in
     * <tt>NNormalSurfaceList</tt>.
     * @param surface the normal surface whose coordinates we are examining.
     * @param coord the coordinate to examine; this should be between 0
     * and one less than the total number of coordinates for the given
     * flavour of coordinate system (inclusive).
     * @return the requested coordinate in the given normal surface.
     * @see normal.engine.surfaces.NNormalSurfaceList
     */
    public static BigInteger getCoordinate(int flavour,
            NNormalSurface surface, int coord) {
        int tmp;
        switch(flavour) {
            case NNormalSurfaceList.STANDARD:
                tmp = coord % 7;
                if (tmp < 4)
                    return surface.getTriangleCoord(coord / 7, tmp);
                else
                    return surface.getQuadCoord(coord / 7, tmp - 4);
            case NNormalSurfaceList.AN_STANDARD:
                tmp = coord % 10;
                if (tmp < 4)
                    return surface.getTriangleCoord(coord / 10, tmp);
                else if (tmp < 7)
                    return surface.getQuadCoord(coord / 10, tmp - 4);
                else
                    return surface.getOctCoord(coord / 10, tmp - 7);
            case NNormalSurfaceList.QUAD:
                return surface.getQuadCoord(coord / 3, coord % 3);
            case NNormalSurfaceList.EDGE_WEIGHT:
                return surface.getEdgeWeight(coord);
            case NNormalSurfaceList.FACE_ARCS:
                return surface.getFaceArcs(coord / 3, coord % 3);
        }
        return null;
    }

    /**
     * The constant representing this flavour of coordinate system.
     */
    private int flavour;
    
    /**
     * Creates a new coordinate system flavour object.
     *
     * @param flavour the constant representing this flavour of
     * coordinate system; this should be one of the coordinate system
     * constants defined in <tt>NNormalSurfaceList</tt>.
     * @see normal.engine.surfaces.NNormalSurfaceList
     */
    public Coordinates(int flavour) {
        this.flavour = flavour;
    }

    /**
     * Returns the constant representing this flavour of coordinate
     * system.  This is the constant that should be passed to the
     * <tt>NNormalSurfaceList</tt> constructor.
     *
     * @return the constant representing this coordinate system.
     * @see normal.engine.Engine#newNNormalSurfaceList
     */
    public int getFlavour() {
        return flavour;
    }

    /**
     * Returns the name of this flavour of coordinate system.  This
     * is the string that should be displayed in a combo box or the like.
     *
     * @return the name of this coordinate system.
     */
    public String toString() {
        return coordinateSystemName(flavour);
    }

    /**
     * Determines if this and the given object represent the same
     * flavour of coordinate system.
     *
     * @param other the object to compare with this.
     * @return <tt>true</tt> if and only if this and the given object
     * represent the same flavour of coordinate system.
     */
    public boolean equals(Object other) {
        if (! (other instanceof Coordinates))
            return false;
        return (flavour == ((Coordinates)other).flavour);
    }
}
