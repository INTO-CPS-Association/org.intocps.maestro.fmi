/*
* This file is part of the INTO-CPS toolchain.
*
* Copyright (c) 2017-CurrentYear, INTO-CPS Association,
* c/o Professor Peter Gorm Larsen, Department of Engineering
* Finlandsgade 22, 8200 Aarhus N.
*
* All rights reserved.
*
* THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
* THIS INTO-CPS ASSOCIATION PUBLIC LICENSE VERSION 1.0.
* ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
* RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL 
* VERSION 3, ACCORDING TO RECIPIENTS CHOICE.
*
* The INTO-CPS toolchain  and the INTO-CPS Association Public License 
* are obtained from the INTO-CPS Association, either from the above address,
* from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain distribution.
* GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
*
* This program is distributed WITHOUT ANY WARRANTY; without
* even the implied warranty of  MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH IN THE
* BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF
* THE INTO-CPS ASSOCIATION.
*
* See the full INTO-CPS Association Public License conditions for more details.
*/

/*
* Author:
*		Kenneth Lausdahl
*/

package org.intocps.fmi.jnifmuapi;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Source;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.intocps.fmi.jnifmuapi.xml.NamedNodeMapIterator;
import org.intocps.fmi.jnifmuapi.xml.NodeIterator;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class FmuModelDescription
{

	private static final boolean DEBUG = false;
	final private File file;
	final Document doc;
	final XPath xpath;

	public FmuModelDescription(File file) throws ParserConfigurationException,
			SAXException, IOException
	{
		this.file = file;

		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		factory.setNamespaceAware(true);
		DocumentBuilder builder = factory.newDocumentBuilder();

		doc = builder.parse(file);
		XPathFactory xPathfactory = XPathFactory.newInstance();
		xpath = xPathfactory.newXPath();

	}

	public String getModelId() throws XPathExpressionException
	{
		Node name = lookupSingle(doc, xpath, "fmiModelDescription/@modelName");
		return name.getNodeValue();
	}

	public String getGuid() throws XPathExpressionException
	{
		Node name = lookupSingle(doc, xpath, "fmiModelDescription/@guid");
		return name.getNodeValue();
	}

	public boolean validate() throws FileNotFoundException
	{
		return validateAgainstXSD(new FileInputStream(file), new File("modeldescription").listFiles());
	}

	static Node lookupSingle(Object doc, XPath xpath, String expression)
			throws XPathExpressionException
	{
		NodeList list = lookup(doc, xpath, expression);
		if (list != null)
		{
			return list.item(0);
		}
		return null;
	}

	static NodeList lookup(Object doc, XPath xpath, String expression)
			throws XPathExpressionException
	{
		XPathExpression expr = xpath.compile(expression);

		if (DEBUG)
		{
			System.out.println("Starting from: " + formateNodeWithAtt(doc));
		}
		final NodeList list = (NodeList) expr.evaluate(doc, XPathConstants.NODESET);

		if (DEBUG)
		{
			System.out.print("\tFound: ");
		}
		boolean first = true;
		for (Node n : new NodeIterator(list))
		{
			if (DEBUG)
			{
				System.out.println((!first ? "\t       " : "")
						+ formateNodeWithAtt(n));
			}
			first = false;
		}
		if (first)
		{
			if (DEBUG)
			{
				System.out.println("none");
			}
		}
		return list;

	}

	public static String formateNodeWithAtt(Object o)
	{
		if (o instanceof Document)
		{
			return "Root document";
		} else if (o instanceof Node)
		{
			Node node = (Node) o;

			String tmp = "";
			tmp = node.getLocalName();
			if (node.hasAttributes())
			{
				for (Node att : new NamedNodeMapIterator(node.getAttributes()))
				{
					tmp += " " + att + ", ";
				}
			}
			return tmp;
		}
		return o.toString();
	}

	static boolean validateAgainstXSD(InputStream xml, File... xsd)
	{
		try
		{
			SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);

			Source[] sources = new Source[xsd.length];
			for (int i = 0; i < xsd.length; i++)
			{
				sources[i] = new StreamSource(xsd[i]);
			}

			Schema schema = factory.newSchema(sources);

			Document document;
			DocumentBuilderFactory docBuilderFactory = DocumentBuilderFactory.newInstance();
			docBuilderFactory.setNamespaceAware(true);
			document = docBuilderFactory.newDocumentBuilder().parse(xml);

			// Validate using main schema
			schema.newValidator().validate(new DOMSource(document));

			Validator validator = schema.newValidator();
			validator.validate(new StreamSource(xml));
			return true;
		} catch (Exception ex)
		{
			return false;
		}
	}

}
