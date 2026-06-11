<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version='1.0'>

<xsl:template match="*">
  <!-- xsl:message>
    <xsl:text>No template matches </xsl:text>
    <xsl:value-of select="name(.)"/>
    <xsl:text>.</xsl:text>
  </xsl:message -->
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="keywordset/keyword">
  <entry header="10">
    <xsl:apply-templates/>
  </entry>
</xsl:template>

<xsl:template match="book/title|bookinfo/title|abstract">
  <entry header="5">
    <xsl:apply-templates/>
  </entry>
</xsl:template>

<xsl:template match="chapter/title">
  <entry header="4">
    <xsl:apply-templates/>
  </entry>
</xsl:template>

<xsl:template match="sect1/title">
  <entry header="3">
    <xsl:apply-templates/>
  </entry>
</xsl:template>

<xsl:template match="sect*/title|emphasis">
  <entry header="2">
    <xsl:apply-templates/>
  </entry>
</xsl:template>

<xsl:template match="application|guilabel|guiicon|menuchoice|guibutton|guisubmenu|guimenuitem|term|guimenu|literal|acronym">
 <entry header="1">
    <xsl:apply-templates/>
 </entry>
</xsl:template>

<xsl:template match="book">
  <entry header="0">
    <xsl:apply-templates/>
  </entry>
</xsl:template>

<xsl:template match="date|releaseinfo|anchor|imagedata">
</xsl:template>

</xsl:stylesheet>
