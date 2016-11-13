<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

   <xsl:import href="../docbook/xsl/html/autoidx.xsl"/>
   <xsl:import href="../docbook/xsl/html/chunk.xsl"/>

   <xsl:param name="local.l10n.xml" select="document(concat('xsl/',/*/@lang,'.xml'))"/>

   <xsl:template name="generate.html.title"/>
</xsl:stylesheet>
