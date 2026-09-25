import xml.etree.ElementTree as ET

tree = ET.parse('mainwindow.ui')
root = tree.getroot()

widget = root.find(".//widget[@name='centralwidget']")

# Check if connectivity_box already exists
if not root.find(".//widget[@name='connectivity_box']"):
    new_widgets_xml = """
    <root>
       <widget class="QComboBox" name="connectivity_box">
        <property name="geometry">
         <rect>
          <x>790</x>
          <y>270</y>
          <width>80</width>
          <height>40</height>
         </rect>
        </property>
        <item>
         <property name="text">
          <string>4-Way</string>
         </property>
        </item>
        <item>
         <property name="text">
          <string>8-Way</string>
         </property>
        </item>
       </widget>
    </root>
    """
    new_widgets = ET.fromstring(new_widgets_xml)
    for child in new_widgets:
        widget.append(child)

    tree.write('mainwindow.ui')
