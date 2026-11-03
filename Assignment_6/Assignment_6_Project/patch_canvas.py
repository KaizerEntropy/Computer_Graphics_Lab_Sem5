import re

with open('canvas.cpp', 'r') as f:
    content = f.read()

# 1. Update paintEvent
paint_event_orig = """    if (m_showClipResult && !m_csResults.isEmpty()) drawClippedLines(painter);
    if (m_showClipResult && !m_shResult.clippedVertices.isEmpty()) drawClippedPolygon(painter);"""
paint_event_new = """    if (m_showClipResult) {
        if (!m_csResults.isEmpty() || !m_lbResults.isEmpty()) drawClippedLines(painter);
        if (!m_shResult.clippedVertices.isEmpty()) drawClippedPolygon(painter);
        if (!m_waResult.disjointPolygons.isEmpty()) drawWAClippedPolygon(painter);
    }"""
content = content.replace(paint_event_orig, paint_event_new)

# 2. Update runCohenSutherland
run_cs_orig = """void Canvas::runCohenSutherland() {
    m_csResults.clear();"""
run_cs_new = """void Canvas::runCohenSutherland() {
    m_csResults.clear();
    m_lbResults.clear();"""
content = content.replace(run_cs_orig, run_cs_new)

# 3. Update runLiangBarsky
run_lb_orig = """void Canvas::runLiangBarsky() {
    m_lbResults.clear();"""
run_lb_new = """void Canvas::runLiangBarsky() {
    m_lbResults.clear();
    m_csResults.clear();"""
content = content.replace(run_lb_orig, run_lb_new)

# 4. Update runWeilerAtherton
run_wa_orig = """void Canvas::runWeilerAtherton() {
    QVector<QPointF> vertices;"""
run_wa_new = """void Canvas::runWeilerAtherton() {
    m_waResult.disjointPolygons.clear();
    m_shResult.stages.clear();
    m_shResult.clippedVertices.clear();
    QVector<QPointF> vertices;"""
content = content.replace(run_wa_orig, run_wa_new)

# 5. Update runSutherlandHodgman
run_sh_orig = """void Canvas::runSutherlandHodgman() {
    // Use the first polygon or the selected polygon"""
run_sh_new = """void Canvas::runSutherlandHodgman() {
    m_waResult.disjointPolygons.clear();
    m_shResult.stages.clear();
    m_shResult.clippedVertices.clear();
    // Use the first polygon or the selected polygon"""
content = content.replace(run_sh_orig, run_sh_new)

# 6. Update drawClippedLines to handle m_lbResults
draw_clipped_lines_orig = """    for (const auto &res : m_csResults) {
        QPoint p1(qRound(res.originalP1.x()), qRound(res.originalP1.y()));"""
draw_clipped_lines_new = """    auto drawResultBlock = [&](const auto &res) {
        QPoint p1(qRound(res.originalP1.x()), qRound(res.originalP1.y()));
        QPoint p2(qRound(res.originalP2.x()), qRound(res.originalP2.y()));

        if (res.accepted) {
            QPoint cp1(qRound(res.clippedP1.x()), qRound(res.clippedP1.y()));
            QPoint cp2(qRound(res.clippedP2.x()), qRound(res.clippedP2.y()));

            auto acceptedPts = CGAlgorithms::bresenhamLine(cp1.x(), cp1.y(), cp2.x(), cp2.y());
            for (const auto &pt : acceptedPts) {
                drawPixelCell(pt, QColor(46, 213, 115));
            }

            if (p1 != cp1) {
                auto clippedPts1 = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), cp1.x(), cp1.y());
                for (int i = 0; i < clippedPts1.size(); ++i) {
                    if (i % 2 == 0) drawDottedPixelCell(clippedPts1[i], QColor(255, 71, 87));
                }
            }

            if (cp2 != p2) {
                auto clippedPts2 = CGAlgorithms::bresenhamLine(cp2.x(), cp2.y(), p2.x(), p2.y());
                for (int i = 0; i < clippedPts2.size(); ++i) {
                    if (i % 2 == 0) drawDottedPixelCell(clippedPts2[i], QColor(255, 71, 87));
                }
            }

            drawPixelCell(cp1, QColor(255, 221, 89));
            drawPixelCell(cp2, QColor(255, 221, 89));
            drawPixelCell(p1, QColor(255, 71, 87));
            drawPixelCell(p2, QColor(255, 71, 87));
        } else {
            auto allPts = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), p2.x(), p2.y());
            for (int i = 0; i < allPts.size(); ++i) {
                if (i % 2 == 0) drawDottedPixelCell(allPts[i], QColor(255, 71, 87));
            }
            drawPixelCell(p1, QColor(255, 71, 87));
            drawPixelCell(p2, QColor(255, 71, 87));
        }
    };

    if (!m_csResults.isEmpty()) {
        for (const auto &res : m_csResults) drawResultBlock(res);
    } else if (!m_lbResults.isEmpty()) {
        for (const auto &res : m_lbResults) drawResultBlock(res);
    }
"""
content = re.sub(r'    for \(const auto &res : m_csResults\) \{.*?(?=\n    }\n    painter\.restore\(\);\n\})', draw_clipped_lines_new.strip(), content, flags=re.DOTALL)

# 7. Add drawWAClippedPolygon at the end of drawClippedPolygon
wa_renderer = """
void Canvas::drawWAClippedPolygon(QPainter &painter) {
    if (m_waResult.disjointPolygons.isEmpty()) return;

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);

    auto drawPixelCell = [&](const QPoint &logicalPt, const QColor &color, bool border = true) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (border && m_gridSize >= 6) {
            painter.setPen(color.darker(140));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

    auto drawDottedPixelCell = [&](const QPoint &logicalPt, const QColor &color) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (m_gridSize >= 6) {
            painter.setPen(QColor(255, 255, 255, 180));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

    // Draw original polygon boundary as DOTTED
    const auto &orig = m_waResult.originalVertices;
    int on = orig.size();
    if (on >= 3) {
        for (int i = 0; i < on; ++i) {
            QPoint v1(qRound(orig[i].x()), qRound(orig[i].y()));
            QPoint v2(qRound(orig[(i + 1) % on].x()), qRound(orig[(i + 1) % on].y()));
            auto edgePts = CGAlgorithms::bresenhamLine(v1.x(), v1.y(), v2.x(), v2.y());
            for (int j = 0; j < edgePts.size(); ++j) {
                if (j % 2 == 0) drawDottedPixelCell(edgePts[j], QColor(255, 107, 129));
            }
            drawPixelCell(v1, QColor(255, 107, 129));
        }
    }

    // Draw clipped disjoint polygons
    for (const auto &polyVerts : m_waResult.disjointPolygons) {
        QVector<QPoint> clippedIntVerts;
        for (const auto &v : polyVerts) clippedIntVerts.append(QPoint(qRound(v.x()), qRound(v.y())));

        if (clippedIntVerts.size() >= 3) {
            auto fillRes = CGAlgorithms::scanlineFill(clippedIntVerts);
            for (const auto &pt : fillRes.filledPixels) drawPixelCell(pt, QColor(46, 213, 115, 120), false);

            QPolygon poly;
            for (const auto &v : polyVerts) poly << logicalToScreenF(v);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(46, 213, 115, 75));
            painter.drawPolygon(poly);

            int n = clippedIntVerts.size();
            for (int i = 0; i < n; ++i) {
                auto edgePts = CGAlgorithms::bresenhamLine(clippedIntVerts[i].x(), clippedIntVerts[i].y(),
                                                           clippedIntVerts[(i + 1) % n].x(), clippedIntVerts[(i + 1) % n].y());
                for (const auto &pt : edgePts) drawPixelCell(pt, QColor(46, 213, 115));
                drawPixelCell(clippedIntVerts[i], QColor(255, 221, 89));
            }
        }
    }

    painter.restore();
}
"""
content = re.sub(r'(void Canvas::drawClippedPolygon.*? painter\.restore\(\);\n\})', r'\1\n' + wa_renderer, content, flags=re.DOTALL)

with open('canvas.cpp', 'w') as f:
    f.write(content)
