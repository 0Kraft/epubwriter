/**
 * Very simple basic rule set
 *
 * Allows
 *    <i>, <em>, <b>, <strong>, <p>, <div>, <a href="http://foo"></a>, <br>, <span>, <ol>, <ul>, <li>
 *
 * For a proper documentation of the format check advanced.js
 */
var wysihtml5ParserRules = {
  tags: {
  
	body:	{},
	head:	{},
	html:	{},
	title:	{},
	
	abbr:	{},
	acronym:{},
	blockquote:{},
	br:{},
	cite:{},
	code:{},
	dfn:{},
	div:{},
	em:{},
	h1:{},
	h2:{},
	h3:{},
	h4:{},
	h5:{},
	h6:{},
	kbd:{},
	p:{},
	pre:{},
	q:{},
	samp:{},
	span:{},
	strong:{},
	
	a:      {
      set_attributes: {
        target: "_blank",
        rel:    "nofollow"
      },
	   check_attributes: {
        href:   "url" // important to avoid XSS
      }
    },
	  
	ul:     {},
    ol:     {},
    li:     {},
	dl:		{},
	dt:		{},
	dd:		{},
	object:	{},
	param:	{},
	b:      {},
	i:      {},
	big:      {},
	hr:      {},
	small:      {},
	sub:      {},
	sup:      {},
	tt:      {},
	del:		{},
	ins:		{},
	bdo:		{},
	button:		{}, 
	fieldsetbdo:{},
	form:		{},
	input:		{},
	label:		{},
	legend:		{},
	select:		{},
	optgroup:		{},
	option:		{}, 
	textarea:		{},
	caption:		{},
	col:		{},
	colgroup:		{},
	table:		{},
	tbody:		{},
	td:		{},
	tfoot:		{},
	th:		{},
	thead:		{},
	tr:		{},
	img:	 {},
      
	
	area:	{},
	map:	{},
	meta:	{},
	noscript:		{},
	script:		{},
	style:		{},
	link:		{},
	base:		{}
	
   
  }
};