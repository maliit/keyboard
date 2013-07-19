#!/usr/bin/perl

##
## This file is part of Maliit Plugins
##
## Copyright (C) 2012 Openismus GmbH. All rights reserved.
##
## Contact: maliit-discuss@lists.maliit.org
##
## Redistribution and use in source and binary forms, with or without modification,
## are permitted provided that the following conditions are met:
##
## Redistributions of source code must retain the above copyright notice, this list
## of conditions and the following disclaimer.
## Redistributions in binary form must reproduce the above copyright notice, this list
## of conditions and the following disclaimer in the documentation and/or other materials
## provided with the distribution.
## Neither the name of Nokia Corporation nor the names of its contributors may be
## used to endorse or promote products derived from this software without specific
## prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
## EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
## MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
## THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
## EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
## SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
## HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
## OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
## SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##

# This script converts language layout files with XML format into new
# XML format.
# Usage:
# ./update-langfile.pl [file1.xml [file2.xml ...]]
#
# Produces file1.xml.new.xml and file2.xml.new.xml.
#
# If you are happy with the changes made by this script, go to the
# directory with layout files and run in shell:
# for new in *.new.xml; do old=`basename "${new}" '.new.xml'`; mv "${new}" "${old}"; done

# Short description of what this script do:
# In first pass it builds a tree of tags from input file. In second
# pass it goes through the tree and calls print_to_file method on each
# tree element. The print_to_file prints the new contents basing on
# current element and, possibly, on its children.

use strict;
use warnings;
use v5.12;

use XML::Parser::Expat;
use XML::Writer;
use constant {START_INDEX => 0,
              END_INDEX => 1};

package Data;

sub new
{
    my ($type) = @_;
    my $class = (ref ($type) or $type or 'Data');
    my $self = {'tree' => [],
                'stack' => []};

    return bless ($self, $class);
}

sub get_tree
{
    my ($self) = @_;

    return $self->{'tree'};
}

sub get_stack
{
    my ($self) = @_;

    return $self->{'stack'};
}

package BaseNode;

sub new
{
    my ($type) = @_;
    my $class = (ref ($type) or $type or 'BaseNode');
    my $self = {};

    return bless ($self, $class);
}

sub print_to_file
{
    die "Not implemented";
}

package XMLDecl;

use parent -norequire, qw(BaseNode);

sub new
{
    my ($type, $version, $encoding, $standalone) = @_;
    my $class = (ref ($type) or ($type) or 'XMLDecl');
    my $self = $class->SUPER::new ();

    $self->{'version'} = $version;
    $self->{'encoding'} = $encoding;
    $self->{'standalone'} = $standalone;

    return bless ($self, $class);
}

sub print_to_file
{
    my ($self, $writer) = @_;
    my $version = $self->{'version'};
    my $encoding = $self->{'encoding'};
    my $standalone = $self->{'standalone'};

    $writer->xmlDecl ($encoding, $standalone);
}

package DocType;

use parent -norequire, qw(BaseNode);

sub new
{
    my ($type, $name, $sysid, $pubid, $internal) = @_;
    my $class = (ref ($type) or $type or 'DocType');
    my $self = $class->SUPER::new ();

    $self->{'name'} = $name;
    $self->{'sysid'} = $sysid;
    $self->{'pubid'} = $pubid;
    $self->{'internal'} = $internal;

    return bless ($self, $class);
}

sub print_to_file
{
    my ($self, $writer) = @_;
    my $name = $self->{'name'};
    my $sysid = $self->{'sysid'};
    my $pubid = $self->{'pubid'};
    my $internal = $self->{'internal'};

    $writer->doctype ($name, $pubid, $sysid);
}

package Tag;

use parent -norequire, qw(BaseNode);

sub new
{
    my ($type, $name, $attributes) = @_;
    my $class = (ref ($type) or $type or 'Tag');
    my $self = $class->SUPER::new ();

    $self->{'name'} = $name;
    $self->{'attributes'} = $attributes;
    $self->{'kids'} = [];

    return bless ($self, $class);
}

sub get_name
{
    my ($self) = @_;

    return $self->{'name'};
}

sub get_attributes
{
    my ($self) = @_;

    return $self->{'attributes'};
}

sub get_kids
{
    my ($self) = @_;

    return $self->{'kids'};
}

sub sorted_attributes
{
    my ($self) = @_;
    my $attributes = $self->get_attributes ();
    my $array = [];

    foreach my $key (sort (keys (%{$attributes})))
    {
        push (@{$array}, $key, $attributes->{$key});
    }

    return $array;
}

sub open_tag
{
    my ($self, $writer) = @_;
    my $name = $self->get_name ();
    my $sorted = $self->sorted_attributes ();

    $writer->startTag ($name, @{$sorted});
}

sub close_tag
{
    my ($self, $writer) = @_;
    my $name = $self->get_name ();

    $writer->endTag ($name);
}

sub standalone_tag
{
    my ($self, $writer) = @_;
    my $name = $self->get_name ();
    my $sorted = $self->sorted_attributes ();

    $writer->emptyTag ($name, @{$sorted});
}

sub print_to_file
{
    my ($self, $writer) = @_;
    my $kids = $self->get_kids ();

    if (@{$kids} > 0)
    {
        $self->open_tag ($writer);
        foreach my $tag (@{$kids})
        {
            $tag->print_to_file ($writer);
        }
        $self->close_tag ($writer);
    }
    else
    {
        $self->standalone_tag ($writer);
    }
}

sub add_kid
{
    my ($self, $kid) = @_;
    my $kids = $self->get_kids ();

    push (@{$kids}, $kid);
}

package Key;

use parent -norequire, qw(Tag);

sub new
{
    my ($type, $name, $attributes) = @_;
    my $class = (ref ($type) or $type or 'Key');
    my $self = $class->SUPER::new ($name, $attributes);

    $self->{'new_kids'} = [];
    $self->{'normal_binding'} = undef;
    $self->{'pending_mods'} = [];

    return bless ($self, $class);
}

sub get_new_kids
{
    my ($self) = @_;

    return $self->{'new_kids'};
}

sub add_new_kid
{
    my ($self, $kid) = @_;
    my $new_kids = $self->get_new_kids ();

    push (@{$new_kids}, $kid);
}

sub get_normal_binding
{
    my ($self) = @_;

    return $self->{'normal_binding'};
}

sub set_normal_binding
{
    my ($self, $binding) = @_;

    $self->{'normal_binding'} = $binding;
}

sub get_pending_mods
{
    my ($self) = @_;

    return $self->{'pending_mods'};
}

sub process_extended_labels
{
    my ($self, $extended_labels, $modifier) = @_;

    if ($extended_labels ne '')
    {
        my $extended_tag = undef;

        {
            my $new_key_kids = $self->get_new_kids ();

            foreach my $new_key_kid (@{$new_key_kids})
            {
                if ($new_key_kid->get_name () eq 'extended')
                {
                    $extended_tag = $new_key_kid;
                    last;
                }
            }
        }

        unless (defined ($extended_tag))
        {
            $extended_tag = Tag->new ('extended',
                                      {});
            $self->add_new_kid ($extended_tag);
        }

        my $row_tag = undef;

        {
            my $ext_kids = $extended_tag->get_kids ();

            if (@{$ext_kids})
            {
                $row_tag = $ext_kids->[0];
            }
        }

        unless (defined ($row_tag))
        {
            $row_tag = Tag->new ('row',
                                 {});
            $extended_tag->add_kid ($row_tag);
        }

        my @extended_chars = split (//, $extended_labels);
        my $index = 0;
        my $row_kids = $row_tag->get_kids ();

        foreach my $char (@extended_chars)
        {
            #say ('|' . $char . '|');

            until (@{$row_kids} > $index)
            {
                my $temp_key = Tag->new ('key', {});

                $row_tag->add_kid ($temp_key);
            }

            my $ext_key = $row_kids->[$index];
            my $binding = Tag->new ('binding', { 'label' => $char });
            my $ext_key_kids = $ext_key->get_kids ();

            if ($modifier eq '')
            {
                if (@{$ext_key_kids} > 0)
                {
                    $ext_key_kids->[0]->get_attributes ()->{'label'} = $char;
                }
                else
                {
                    $ext_key->add_kid ($binding);
                }
            }
            else
            {
                unless (@{$ext_key_kids} > 0)
                {
                    my $temp_binding = Tag->new ('binding', {});

                    $ext_key->add_kid ($temp_binding);
                }

                my $parent_binding = $ext_key_kids->[0];
                my $mod_tag = undef;

                foreach my $any_mod_tag (@{$parent_binding->get_kids ()})
                {
                    if ($any_mod_tag->get_attributes ()->{'keys'} eq $modifier)
                    {
                        $mod_tag = $any_mod_tag;
                        last;
                    }
                }

                unless (defined ($mod_tag))
                {
                    $mod_tag = Tag->new ('modifiers', {'keys' => $modifier});
                    $parent_binding->add_kid ($mod_tag);
                }

                $mod_tag->add_kid ($binding);
            }

            ++$index;
        }
    }
}

sub process_modifier
{
    my ($self, $label, $modifier, $binding_tag) = @_;

    if ($modifier eq '')
    {
        $self->set_normal_binding ($binding_tag);
    }
    else
    {
        my $mod_tag = Tag->new ('modifiers',
                                {'keys' => $modifier});
        my $normal_binding = $self->get_normal_binding ();

        $mod_tag->add_kid ($binding_tag);

        if (defined ($normal_binding))
        {
            $normal_binding->add_kid ($mod_tag);
        }
        else
        {
            my $pending_mods = $self->get_pending_mods ();

            push (@{$pending_mods}, $mod_tag);
        }
    }
}

sub print_to_file
{
    my ($self, $writer) = @_;
    my $kids = $self->get_kids ();

    if (@{$kids} > 0)
    {
        $self->open_tag ($writer);
        foreach my $tag (@{$kids})
        {
            my $put_it_in = 1;

            if ($tag->get_name () eq 'binding')
            {
                my $binding_attrs = $tag->get_attributes ();
                my $e_l = 'extended_labels';
                my $a = 'alt';
                my $s = 'shift';
                my $modifier = '';
                my $extended_labels = '';

                if (exists ($binding_attrs->{$a}))
                {
                    if (delete ($binding_attrs->{$a}) eq 'true')
                    {
                        $modifier .= $a;
                    }
                }
                if (exists ($binding_attrs->{$s}))
                {
                    if (delete ($binding_attrs->{$s}) eq 'true')
                    {
                        $modifier .= $s;
                    }
                }
                if (exists ($binding_attrs->{$e_l}))
                {
                    $extended_labels = delete ($binding_attrs->{$e_l});
                }

                $put_it_in = ($modifier eq '');

                $self->process_modifier ($binding_attrs->{'label'},
                                         $modifier,
                                         $tag);
                $self->process_extended_labels ($extended_labels,
                                                $modifier);
            }
            if ($put_it_in)
            {
                $self->add_new_kid ($tag);
            }
        }

        my $pending_mods = $self->get_pending_mods ();
        my $normal_binding_tag = $self->get_normal_binding ();
        my $new_kids = $self->get_new_kids ();

        foreach my $mod_tag (@{$pending_mods})
        {
            die unless (defined ($normal_binding_tag));

            $normal_binding_tag->add_kid ($mod_tag);
        }
        foreach my $tag (@{$new_kids})
        {
            $tag->print_to_file ($writer);
        }
        $self->close_tag ($writer);
    }
    else
    {
        $self->standalone_tag ($writer);
    }
}

package main;

sub default_start_handler
{
    my ($data, undef, $tag, $attributes) = @_;
    my $stack = $data->get_stack ();
    my $node = Tag->new ($tag, $attributes);
    my $parent_node = $stack->[-1];

    $parent_node->add_kid ($node);
    push (@{$stack}, $node);
}

sub default_end_handler
{
    my ($data) = @_;
    my $stack = $data->get_stack ();

    pop (@{$stack});
}

sub keyboard_start
{
    my ($data, undef, $tag, $attributes) = @_;
    my $stack = $data->get_stack ();
    my $tree = $data->get_tree ();
    my $node = Tag->new ($tag, $attributes);

    die if (@{$stack} > 0);
    push (@{$stack}, $node);
    push (@{$tree}, $node);
}

sub keyboard_end
{
    my ($data) = @_;
    my $stack = $data->get_stack ();

    die if (@{$stack} != 1);

    default_end_handler (@_);
}

sub start_key
{
    my ($data, $parser, $tag, $attributes) = @_;
    my $stack = $data->get_stack ();
    my $node = Key->new ($tag, $attributes);
    my $parent_node = $stack->[-1];

    $parent_node->add_kid ($node);
    push (@{$stack}, $node);
}

my %handlers =
(
  'keyboard' => [\&keyboard_start, \&keyboard_end],
  'key' => [\&start_key, \&default_end_handler]
);

sub start
{
    my ($data, $parser, $tag, %attributes) = @_;
    my $handler = undef;

    if (exists ($handlers{$tag}))
    {
        $handler = $handlers{$tag}[START_INDEX];

    } else {
        $handler = \&default_start_handler;
    }
    &{$handler} ($data, $parser, $tag, \%attributes);
}

sub end
{
    my ($data, $parser, $tag) = @_;
    my $handler = undef;

    if (exists ($handlers{$tag}))
    {
        $handler = $handlers{$tag}[END_INDEX];
    } else {
        $handler = \&default_end_handler;
    }
    &{$handler} ($data, $parser, $tag);
}

sub xml_decl
{
    my ($data, $parser, $version, $encoding, $standalone) = @_;
    my $tree = $data->get_tree ();
    my $node = XMLDecl->new ($version, $encoding, $standalone);

    push (@{$tree}, $node);
}

sub doctype
{
    my ($data, $parser, $name, $sysid, $pubid, $internal) = @_;
    my $tree = $data->get_tree ();
    my $node = DocType->new ($name, $sysid, $pubid, $internal);

    push (@{$tree}, $node);
}

foreach my $filename (@ARGV)
{
    my $file = IO::File->new ($filename, 'r');
    my $fd = IO::File->new ($filename . '.new.xml', 'w');
    my $parser = XML::Parser::Expat->new ();
    my $data = Data->new ();
    my $writer = XML::Writer->new ('OUTPUT' => $fd,
                                   'DATA_MODE' => 1,
                                   'DATA_INDENT' => '  ');

    die unless (defined ($file));
    die unless (defined ($fd));

    binmode ($file, ':utf8');
    binmode ($fd, ':utf8');

    $parser->setHandlers ('Start' => sub { start ($data, @_); },
                          'End' => sub { end ($data, @_); },
                          'XMLDecl' => sub { xml_decl ($data, @_); },
                          'Doctype' => sub { doctype ($data, @_); });
    $parser->parse ($file);
    $file->close ();

    foreach my $node (@{$data->get_tree ()})
    {
        $node->print_to_file ($writer);
    }
    $fd->say ();

    $parser->release ();
    $fd->close();
}
