/***************************************************************************
    qgsvectorlayereditutils.h
    ---------------------
    begin                : Dezember 2012
    copyright            : (C) 2012 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSVECTORLAYEREDITUTILS_H
#define QGSVECTORLAYEREDITUTILS_H


#include "qgsfeature.h"

#include "qgsvectorlayer.h"

class QgsGeometryCache;
class QgsCurveV2;

class CORE_EXPORT QgsVectorLayerEditUtils
{
  public:
    QgsVectorLayerEditUtils( QgsVectorLayer* layer );

    inline QgsGeometryCache* cache() { return L->cache(); }


    /** Insert a new vertex before the given vertex number,
     *  in the given ring, item (first number is index 0), and feature
     *  Not meaningful for Point geometries
     */
    bool insertVertex( double x, double y, QgsFeatureId atFeatureId, int beforeVertex );

    /** Moves the vertex at the given position number,
     *  ring and item (first number is index 0), and feature
     *  to the given coordinates
     */
    bool moveVertex( double x, double y, QgsFeatureId atFeatureId, int atVertex );

    /** Moves the vertex at the given position number,
     *  ring and item (first number is index 0), and feature
     *  to the given coordinates
     *  @note available in python bindings as moveVertexV2
     */
    bool moveVertex( const QgsPointV2& p, QgsFeatureId atFeatureId, int atVertex );

    /** Deletes a vertex from a feature
     * @deprecated use deleteVertexV2() instead
     */
    Q_DECL_DEPRECATED bool deleteVertex( QgsFeatureId atFeatureId, int atVertex );

    /** Deletes a vertex from a feature.
     * @param featureId ID of feature to remove vertex from
     * @param vertex index of vertex to delete
     * @note added in QGIS 2.14
     */
    //TODO QGIS 3.0 - rename to deleteVertex
    QgsVectorLayer::EditResult deleteVertexV2( QgsFeatureId featureId, int vertex );

    /** Adds a ring to polygon/multipolygon features
     * @param ring ring to add
     * @param targetFeatureIds if specified, only these features will be the candidates for adding a ring. Otherwise
     * all intersecting features are tested and the ring is added to the first valid feature.
     * @param modifiedFeatureId if specified, feature ID for feature that ring was added to will be stored in this parameter
     * @return
     *   0 in case of success,
     *   1 problem with feature type,
     *   2 ring not closed,
     *   3 ring not valid,
     *   4 ring crosses existing rings,
     *   5 no feature found where ring can be inserted
     */
    // TODO QGIS 3.0 returns an enum instead of a magic constant
    int addRing( const QList<QgsPoint>& ring, const QgsFeatureIds& targetFeatureIds = QgsFeatureIds(), QgsFeatureId* modifiedFeatureId = nullptr );

    /** Adds a ring to polygon/multipolygon features
     * @param ring ring to add
     * @param targetFeatureIds if specified, only these features will be the candidates for adding a ring. Otherwise
     * all intersecting features are tested and the ring is added to the first valid feature.
     * @param modifiedFeatureId if specified, feature ID for feature that ring was added to will be stored in this parameter
     * @return
     *  0 in case of success,
     *  1 problem with feature type,
     *  2 ring not closed,
     *  3 ring not valid,
     *  4 ring crosses existing rings,
     *  5 no feature found where ring can be inserted
     * @note available in python bindings as addCurvedRing
     */
    // TODO QGIS 3.0 returns an enum instead of a magic constant
    int addRing( QgsCurveV2* ring, const QgsFeatureIds& targetFeatureIds = QgsFeatureIds(), QgsFeatureId* modifiedFeatureId = nullptr );

    /** Adds a new part polygon to a multipart feature
     * @return
     *  0 in case of success,
     *  1 if selected feature is not multipart,
     *  2 if ring is not a valid geometry,
     *  3 if new polygon ring not disjoint with existing rings,
     *  4 if no feature was selected,
     *  5 if several features are selected,
     *  6 if selected geometry not found
     */
    // TODO QGIS 3.0 returns an enum instead of a magic constant
    int addPart( const QList<QgsPoint>& ring, QgsFeatureId featureId );

    /** Adds a new part polygon to a multipart feature
     * @return
     *  0 in case of success,
     *  1 if selected feature is not multipart,
     *  2 if ring is not a valid geometry,
     *  3 if new polygon ring not disjoint with existing rings,
     *  4 if no feature was selected,
     *  5 if several features are selected,
     *  6 if selected geometry not found
     * @note available in python bindings as addPartV2
     */
    // TODO QGIS 3.0 returns an enum instead of a magic constant
    int addPart( const QgsPointSequenceV2 &ring, QgsFeatureId featureId );

    // @note available in python bindings as addCurvedPart
    // TODO QGIS 3.0 returns an enum instead of a magic constant
    int addPart( QgsCurveV2* ring, QgsFeatureId featureId );

    /** Translates feature by dx, dy
     * @param featureId id of the feature to translate
     * @param dx translation of x-coordinate
     * @param dy translation of y-coordinate
     * @return 0 in case of success
     */
    int translateFeature( QgsFeatureId featureId, double dx, double dy );

    /** Splits parts cut by the given line
     *  @param splitLine line that splits the layer feature parts
     *  @param topologicalEditing true if topological editing is enabled
     *  @return
     *   0 in case of success,
     *   4 if there is a selection but no feature split
     */
    // TODO QGIS 3.0 returns an enum instead of a magic constant
    int splitParts( const QList<QgsPoint>& splitLine, bool topologicalEditing = false );

    /** Splits features cut by the given line
     *  @param splitLine line that splits the layer features
     *  @param topologicalEditing true if topological editing is enabled
     *  @return
     *   0 in case of success,
     *   4 if there is a selection but no feature split
     */
    // TODO QGIS 3.0 returns an enum instead of a magic constant
    int splitFeatures( const QList<QgsPoint>& splitLine, bool topologicalEditing = false );

    /** Adds topological points for every vertex of the geometry.
     * @param geom the geometry where each vertex is added to segments of other features
     * @note geom is not going to be modified by the function
     * @return 0 in case of success
     */
    int addTopologicalPoints( const QgsGeometry *geom );

    /** Adds a vertex to segments which intersect point p but don't
     * already have a vertex there. If a feature already has a vertex at position p,
     * no additional vertex is inserted. This method is useful for topological
     * editing.
     * @param p position of the vertex
     * @return 0 in case of success
     */
    int addTopologicalPoints( const QgsPoint& p );

    /** Inserts vertices to the snapped segments.
     * This is useful for topological editing if snap to segment is enabled.
     * @param snapResults results collected from the snapping operation
     * @return 0 in case of success
     */
    int insertSegmentVerticesForSnap( const QList<QgsSnappingResult>& snapResults );

  protected:

    /** Little helper function that gives bounding box from a list of points.
    @return 0 in case of success */
    int boundingBoxFromPointList( const QList<QgsPoint>& list, double& xmin, double& ymin, double& xmax, double& ymax ) const;

    QgsVectorLayer* L;
};

#endif // QGSVECTORLAYEREDITUTILS_H
